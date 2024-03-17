#include "Parser.hh"

#include <iostream>
#include <queue>
#include <stack>
#include <filesystem>

namespace mana {
    Parser::Parser()
    {
    }

    void Parser::registerErrorCallback(ErrorCallback_T callback)
    {
        g_errorCallback = callback;
    }

    void Parser::parse(const std::string& code) 
    {
        m_tokenIdx = -1;

        m_stats.lineIndex = 1;
        m_stats.columnIndex = 0;

        m_lexer = std::make_unique<Lexer>(code);

        if (m_lexer->size() > 0)
            if ((*m_lexer)[0].kind == Token::Type::kWS || (*m_lexer)[0].kind == Token::Type::kLnBrk) {
                    m_tokenIdx--;
            }

        std::vector<std::unique_ptr<ast::TreeNode>> tr_nodes;
        
        while (continueParsing()) {
            globalDeclaration();
        }

        std::ostream& stream { std::cout };

        stream << "AST Print:" << std::endl;

        for (auto& it : tr_nodes) {
            it->print(stream, 0);
            stream << std::endl;
        }
    }

    size_t Parser::getPrecedence(const Token& tk) {
        switch (tk.kind) {
            case Token::Type::kComma:
                return 0;
            case Token::Type::kMinus:
            case Token::Type::kPlus:
                return 1;
            case Token::Type::kAsterisk:
                return 2;
            case Token::Type::kSlash:
            case Token::Type::kMod:
                return 3;
            case Token::Type::kDualColon: // scope resolution
                return 4;
            default:
                return 0;
        }
    }

    Parser::Associativity Parser::getAssociativity(const Token& tk) {
        switch (tk.kind) {
            case Token::Type::kMinus:
            case Token::Type::kPlus:
            case Token::Type::kAsterisk:
            case Token::Type::kSlash:
            case Token::Type::kDualColon:
            case Token::Type::kComma:
                return Associativity::kLeft;
            default:
                return Associativity::kRight;
        }

        unreachable();
    }

    Result<const ast::Expression*> Parser::primaryExpression()
    {
        auto expr = expression();

        if (expr.errored) {
            return Failure::kError;
        } 

        if (expr.matched) {
            return expr;
        }

        return Failure::kNoMatch;
    }

    Result<const ast::Expression*> Parser::expression()
    {
        auto lit = literal();

        if (lit.errored) {
            return Failure::kError;
        }

        if (lit.matched) {
            return lit;
        }

        return Failure::kNoMatch;
    }

    Result<const ast::Expression*> Parser::expectExpression(bool isExpr) {
        auto result = primaryExpression();

        if (!result.matched || result.errored) return Failure::kError;

        result = parseExpression1(result.unwrap(), 0ull, isExpr);

        return result;
    }

    bool Parser::isOperator(const Token& tok, bool isWithinExpr)
    {
        switch (tok.kind) {
            case Token::Type::kAsterisk:
            case Token::Type::kSlash:
            case Token::Type::kPlus:
            case Token::Type::kMinus:
            case Token::Type::kDualColon:
                return true;
            case Token::Type::kComma:
                if (isWithinExpr) return true;
                else MANA_FATAL_NO_RETURN("COMMA IS NOT AN OPERATOR");
            default:
                return false;
        }

        unreachable();
    }

    Result<const ast::Expression*> Parser::parseExpression1(
        const ast::Expression* lhs,
        size_t min_precedence,
        bool isExpr
    )
    {
        auto* lookahead = peek(1);

        if (lookahead && isOperator(*lookahead, isExpr)) {
            MANA_CHECK_MAYBE_RETURN(lookahead, "Trying to consume end of stream.");

            while (lookahead && isOperator(*lookahead, isExpr) && getPrecedence(*lookahead) >= min_precedence) {
                auto op = lookahead;

                advance();

                auto rhs_expr = primaryExpression();

                if (rhs_expr.errored || !rhs_expr.matched) {
                    return Failure::kError;
                }

                auto* rhs = rhs_expr.unwrap();

                lookahead = peek(1);

                while (lookahead && (
                    isOperator(*lookahead, isExpr) 
                    && (getPrecedence(*lookahead) >= getPrecedence(*op) 
                            || (getAssociativity(*lookahead) == Associativity::kRight 
                                    && getPrecedence(*lookahead) == getPrecedence(*op)))
                ))
                {
                    auto rhs_expr2 = parseExpression1(
                        rhs,
                        getPrecedence(*op) + ((getPrecedence(*lookahead) > getPrecedence(*op)) ? 1 : 0),
                        isExpr
                    );

                    if (rhs_expr2.errored || !rhs_expr2.matched) {
                        return Failure::kError;
                    }

                    rhs = rhs_expr2.unwrap();

                    lookahead = peek(1);
                }

                switch (op->kind) {
                    case Token::Type::kPlus: {
                        lhs = m_ctx.create<ast::BinaryExpression>(
                            std::move(lhs), 
                            ast::BinaryExpression::OpType::kAdd,
                            std::move(rhs)
                        );
                    } break;

                    case Token::Type::kMinus: {
                        lhs = m_ctx.create<ast::BinaryExpression>(
                            std::move(lhs), 
                            ast::BinaryExpression::OpType::kSubtract, 
                            std::move(rhs)
                        );
                    } break;

                    case Token::Type::kSlash: {
                        lhs = m_ctx.create<ast::BinaryExpression>(
                            std::move(lhs),
                            ast::BinaryExpression::OpType::kDivide, 
                            std::move(rhs)
                        );
                    } break;

                    case Token::Type::kAsterisk: {
                        lhs = m_ctx.create<ast::BinaryExpression>(
                            std::move(lhs),
                            ast::BinaryExpression::OpType::kMultiply, 
                            std::move(rhs)
                        );
                    } break;

                    case Token::Type::kDualColon: {
                        lhs = m_ctx.create<ast::BinaryExpression>(
                            std::move(lhs),
                            ast::BinaryExpression::OpType::kScopeResolution,
                            std::move(rhs)
                        );
                    } break;

                    case Token::Type::kComma: {
                        lhs = m_ctx.create<ast::BinaryExpression>(
                            std::move(lhs),
                            ast::BinaryExpression::OpType::kComma, 
                            std::move(rhs)
                        );
                    } break;

                    default:
                        MANA_FATAL_NO_RETURN("Invalid operator type.");
                }
            }

            return lhs;
        } else {
            return lhs;
        }
    }

    Result<std::vector<const ast::Attribute*>> Parser::attributes()
    {
        std::vector<const ast::Attribute*> ats;

        while (continueParsing()) {
            if (!match(1, Token::Type::kAt)) break;
            auto a = parseAttribute();

            if (a.errored) {
                // TODO: Handle error and return
                continue;
            }

            if (a.matched) {
                ats.push_back(a.unwrap());
            }
        }

        return ats;
    }

    Result<const ast::LiteralExpression*> Parser::literal()
    {
        if (auto* tok = match(1, Token::Type::kInteger))
            return m_ctx.create<ast::IntegerLiteralExpression>(
                ast::IntegerLiteralExpression::Sign::kSigned,
                tok->asInteger()
            );
        else if (auto* tok = match(1, Token::Type::kStrLit))
            return m_ctx.create<ast::StringLiteralExpression>(tok->asString());
        else if (auto* tok = match(1, "true")) 
            return m_ctx.create<ast::BoolLiteralExpression>(true);
        else if (auto* tok = match(1, "false"))
            return m_ctx.create<ast::BoolLiteralExpression>(false);

        return Failure::kNoMatch;
    }

    Result<const ast::IdentifierExpression*> Parser::identifierExpression()
    {
        auto* token = match(1, Token::Type::kIdentifier);

        if (token)
            return m_ctx.create<ast::IdentifierExpression>(token->asString());

        return Failure::kNoMatch;
    }

    Result<const ast::Attribute*> Parser::parseAttribute()
    {
        while (continueParsing()) {
            // TODO: Important to review this later
            if (!match(0, Token::Type::kAt)) break;

            auto* attr_name = match(1, Token::Type::kIdentifier);

            if (!attr_name) {
                // TODO: Handle error here
                return Failure::kError;
            }

            const ast::Expression* node;

            if (match(1, Token::Type::kLeftParen)) {
                auto expr = expectExpression();

                if (expr.matched) node = expr.unwrap(); 

                if (!match(1, Token::Type::kRightParen)) {
                    // TODO: Handle error here
                    return Failure::kError;
                }
            }

            /*auto ap = m_ctx.create<ast::Attribute>(
                attr_name->asString(), 
                std::move(node)
            );

            return std::move(ap);*/
        }

        return {};
    }

    bool Parser::isQualifier(Token tok) const
    {
        if (tok.match("readonly"))
            return true;
        else if (tok.match("export"))
            return true;
        return false;
    }

    Result<const ast::UnaryExpression*> Parser::unaryExpression()
    {
        if (match(1, Token::Type::kMinus)) {
            
        } else if (match(1, Token::Type::kPlus)) {

        }

        return {};
    }

    Result<const ast::Expression*> Parser::expectGroup(bool isExpr)
    {
        if (match(1, Token::Type::kLeftParen)) {
            auto expr = expectExpression();

            if (!match(1, Token::Type::kRightParen)) {
                // Handle error here
                return Failure::kError;
            }

            return std::move(expr);
        }

        return {};
    }

    Result<const ast::ComponentDeclaration*> Parser::componentDeclaration()
    {
        bool is_component_exported = false;

        auto as = attributes();

        if (match(1, "export")) {
            is_component_exported = true;
        }

        if (!match(1, "component")) return {};

        auto decl_name = match(1, Token::Type::kIdentifier);

        if (!decl_name) {
            // TODO: Add error message here                
            return Failure::kError;
        }

        std::vector<const ast::IdentifierExpression*> inheritances;

        if (match(1, Token::Type::kColon)) {
            do {
                if (!match(1, Token::Type::kComma)) 
                    return Failure::kError;

                auto iasl = attributes();
                auto unwrapped = iasl.unwrap();

                auto inh = identifierExpression();
                auto inh_unwrapped = inh.unwrap();

                /*for (auto& a : unwrapped)
                    inh_unwrapped->addAttribute(a);*/

                inheritances.push_back(inh_unwrapped);
            } while (match(1, Token::Type::kComma));
        }

        if (!match(1, Token::Type::kLeftBracket)) {
            // TODO: Handle error here                
            return Failure::kError;
        }

        ;

        std::vector<const ast::Declaration*> members;

        while (!match(1, Token::Type::kRightBracket)) {
            auto as = attributes();

            auto type = identifierExpression();

            bool is_optional = false;

            if (match(1, Token::Type::kQuestion)) {
                    is_optional = true;
            }

            auto* field_name = match(1, Token::Type::kIdentifier);

            if (!field_name) {
                // TODO: Handle error here
                return Failure::kError;
            }

            std::string la;

            if (match(1, Token::Type::kArrow)) {
                    auto prop_name = match(1, Token::Type::kIdentifier);

                if (!prop_name) {
                    // TODO: Handle error here
                    return Failure::kError;
                }

                    la = prop_name->asString();
            }

            const ast::Expression* field_default_value;

            if (match(1, Token::Type::kEqual)) {
                auto expr = expectExpression();

                if (expr.errored) {
                    return Failure::kError;
                }

                field_default_value = expr.unwrap();
            }

            for (auto& m : members) {
                if (m->match<ast::MemberDeclaration>()) {
                    auto* member = static_cast<const ast::MemberDeclaration*>(m); 

                    if (field_name->match(member->name())) {
                        // TODO: Handle error here
                        return Failure::kError;
                    }
                }
            }

            auto vuw = type.unwrap();

            auto tree_ = static_cast<const ast::TreeNode*>(vuw);

            members.push_back(
                m_ctx.create<ast::MemberDeclaration>(
                    std::move(vuw),
                    field_name->asString(),
                    std::move(field_default_value),
                    is_optional
                )
            );

            //for (auto& a : as) members.back()->addAttribute(std::move(a));
        }

        if (!match(1, Token::Type::kRightBracket)) {
            // TODO: Handle error here
            return Failure::kError;
        }

        auto comp_decl = m_ctx.create<ast::ComponentDeclaration>(
            decl_name->asString(),
            std::move(members),
            std::move(inheritances),
            is_component_exported
        );

        //for (auto& a : as) comp_decl->addAttribute(std::move(a));

        return comp_decl;
    }

    Result<const ast::ImportDeclaration*> Parser::importDeclaration() {
        if (!match(1, "import")) return {};
        
        bool is_cc = false;
                                                
        std::vector<std::filesystem::path> pathlist;

        if (match(1, Token::Type::kLeftParen)) {
            while (!match(1, Token::Type::kRightParen)) {
                auto* path = match(1, Token::Type::kStrLit);

                if (!path) {
                    // TODO: Handle error here
                    return Failure::kError;
                }

                pathlist.push_back(path->asString());
            }

            if (!match(1, Token::Type::kRightParen)) {
                // TODO: Handle error here.
                return Failure::kError;
            }
        } else {
            const Token* next = match(1, Token::Type::kStrLit);

            if (!next) {
                // TODO: Handle error here.
                return Failure::kError;
            }

            pathlist.push_back(next->asString());
        }

        return m_ctx.create<ast::ImportDeclaration>(
            std::move(pathlist),
            is_cc
        );
    }

    bool Parser::continueParsing()
    {
        return (m_tokenIdx + 1 < m_lexer->size());
    }

    void Parser::globalDeclaration()
    {
        auto decl = sync(Token::Type::kRightBracket, [&]() -> Result<const ast::ComponentDeclaration*> {
            auto cd = componentDeclaration();

            if (cd.errored) {
                // TODO: add error to a list
                return Failure::kError;
            }

            if (cd.matched)
                return cd; 

            return Failure::kNoMatch;
        });

        if (decl.matched) {
            decl->print(std::cout, 0);
        }

        auto import_decl = sync(Token::Type::kRightBracket, [&]() -> Result<const ast::ImportDeclaration*> {
            auto id = importDeclaration();

            if (id.errored)
                return Failure::kError;

            if (id.matched) {
                // TODO: Add this to AST builder
                return id;
            }

            return {};
        });

        if (import_decl.matched) {
            return import_decl->print(std::cout, 0);
        }
    }

    void Parser::checkAttribute()
    {
        auto as = attributes();

        /*if (as.empty()) return;*/

        auto nxt = componentDeclaration();

        if (nxt.errored) {
            // TODO: Handle error here
            return;
        }

        /*if (nxt.matched) {
            for (auto& a : as) 
                nxt->addAttribute(std::move(a));
        }*/
    }
    
    void Parser::doParse()
    {
#       ifdef MANA_IS_VERBOSE
            std::visit([](auto&& arg) {
                std::cout << "Processing token: " << arg << std::endl;
            }, tk->value);
#       endif

        while (continueParsing()) {
            globalDeclaration();

            checkAttribute();
        }
    }

    const Token* Parser::match(int64_t off, Token::Type token_type, bool skip_ws, bool skip_lnbrks)
    {
        auto* token = peek(off, skip_ws, skip_lnbrks);

        if (token && token->kind == token_type) {
            return token;
        }

        return nullptr;
    }

    const Token* Parser::match(int64_t off, std::string_view str, bool skip_ws, bool skip_lnbrks)
    {
        auto* token = peek(off, skip_ws, skip_lnbrks);

        if (!token || token->kind != Token::Type::kIdentifier) return nullptr;

        if (token->match(str)) {
            return token;
        }

        return nullptr;
    }

    const Token* Parser::peek(uint64_t off, bool skip_ws, bool skip_lnbrks) 
    {
        if (off == 0) return &(*m_lexer)[m_tokenIdx];

        uint64_t tokenidx;

        if (m_tokenIdx == m_lexer->size() - 1) return nullptr;

        tokenidx = m_tokenIdx + 1;

        for (; off != 0 && tokenidx < m_lexer->size(); tokenidx++) {
            if (skip_ws && (*m_lexer)[tokenidx].kind == Token::Type::kWS) 
                continue;

            if (skip_lnbrks && (*m_lexer)[tokenidx].kind == Token::Type::kLnBrk)
                continue;
        
            if (--off == 0) break;
        }

        if (off != 0) return nullptr;

        return &(*m_lexer)[tokenidx];   
    }

    const Token* Parser::advance(size_t off, bool skip_ws, bool skip_lnbrks) 
    {
        if (off == 0) 
            return nullptr;

        if (m_tokenIdx == m_lexer->size() - 1) 
            return nullptr;

        m_tokenIdx++;

        while (off != 0 && m_tokenIdx < m_lexer->size()) {
            m_stats.columnIndex += 1;//(*m_lexer)[m_tokenIdx].value.size();

            if ((*m_lexer)[m_tokenIdx].kind == Token::Type::kWS)
                if (skip_ws) {
                    m_tokenIdx++;
                    continue;
                }

            if ((*m_lexer)[m_tokenIdx].kind == Token::Type::kLnBrk) {
                m_stats.columnIndex = 0;
                m_stats.lineIndex++;

                if (skip_lnbrks) {
                    m_tokenIdx++;

                    continue;
                }
            }

            off--;
            
            if (off > 0) m_tokenIdx++;
        }

        if (off != 0) return nullptr;

        return &(*m_lexer)[m_tokenIdx];   
    }
}