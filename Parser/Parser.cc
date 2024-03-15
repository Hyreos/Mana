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
                advance();

                m_tokenIdx--;
            }

        std::vector<std::unique_ptr<ast::TreeNode>> tr_nodes;
        
        while (continueParsing()) {
            auto r = parseExpression();

            if (r) tr_nodes.push_back(std::move(r));
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

    std::unique_ptr<ast::TreeNode> Parser::parseExpression(bool isExpr) {
        std::unique_ptr<ast::TreeNode> result = parsePrimary();;

        result = parseExpression1(std::move(result), 0ull, isExpr);

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

    std::unique_ptr<ast::TreeNode> Parser::parseExpression1(
        std::unique_ptr<ast::TreeNode> lhs,
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

                auto rhs = parsePrimary();

                MANA_CHECK_MAYBE_RETURN(rhs, "Error while parsing token.");

                lookahead = peek(1);

                while (lookahead && (
                    isOperator(*lookahead, isExpr) 
                    && (getPrecedence(*lookahead) >= getPrecedence(*op) 
                            || (getAssociativity(*lookahead) == Associativity::kRight 
                                    && getPrecedence(*lookahead) == getPrecedence(*op)))
                ))
                {
                    rhs = parseExpression1(
                        std::move(rhs),
                        getPrecedence(*op) + ((getPrecedence(*lookahead) > getPrecedence(*op)) ? 1 : 0),
                        isExpr 
                    );

                    lookahead = peek(1);
                }

                switch (op->kind) {
                    case Token::Type::kPlus: {
                        lhs = std::make_unique<ast::SumOp>(std::move(lhs), std::move(rhs));
                    } break;

                    case Token::Type::kMinus: {
                        lhs = std::make_unique<ast::SubOp>(std::move(lhs), std::move(rhs));
                    } break;

                    case Token::Type::kSlash: {
                        lhs = std::make_unique<ast::DivOp>(std::move(lhs), std::move(rhs));
                    } break;

                    case Token::Type::kAsterisk: {
                        lhs = std::make_unique<ast::MulOp>(std::move(lhs), std::move(rhs));
                    } break;

                    case Token::Type::kDualColon: {
                        lhs = std::make_unique<ast::ScopeResolutionOp>(std::move(lhs), std::move(rhs));
                    } break;

                    case Token::Type::kComma: {
                        lhs = std::make_unique<ast::CommaOp>(std::move(lhs), std::move(rhs));
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

    std::vector<std::unique_ptr<ast::Attribute>> Parser::attributes()
    {
        std::vector<std::unique_ptr<ast::Attribute>> ats;

        while (continueParsing()) {
            if (!matches(1, Token::Type::kAt)) break;
            advance();

            auto a = parseAttribute();

            if (a.errored) {
                // TODO: Handle error and return
                continue;
            }

            ats.push_back(std::move(*a.value));
        }

        return ats;
    }

    ASTResult_T<ast::Literal> Parser::checkLiteral()
    {
        if (auto* tok = matches(1, Token::Type::kI16Lit)) {
            advance();
            return ResultOk<ast::Literal>(std::make_unique<ast::Int16Lit>(tok->as16Int()));
        } else if (auto* tok = matches(1, Token::Type::kI32Lit)) {
            advance();
            return ResultOk<ast::Literal>(std::make_unique<ast::Int32Lit>(tok->as32Int()));
        } else if (auto* tok = matches(1, Token::Type::kI64Lit)) {
            advance();
            return ResultOk<ast::Literal>(std::make_unique<ast::Int64Lit>(tok->as64Int()));
        } else if (auto* tok = matches(1, Token::Type::kU16Lit)) {
            advance();
            return ResultOk<ast::Literal>(std::make_unique<ast::Uint16Lit>(tok->as16UInt()));
        } else if (auto* tok = matches(1, Token::Type::kU32Lit)) {
            advance();
            return ResultOk<ast::Literal>(std::make_unique<ast::Uint32Lit>(tok->as32UInt()));
        } else if (auto* tok = matches(1, Token::Type::kU64Lit)) {
            advance();
            return ResultOk<ast::Literal>(std::make_unique<ast::Uint64Lit>(tok->as64UInt()));
        } else if (auto *tok = matches(1, Token::Type::kF32Lit)) {
            advance();
            return ResultOk<ast::Literal>(std::make_unique<ast::Fp32Lit>(tok->asFp32()));
        } else if (auto *tok = matches(1, Token::Type::kF64Lit)) {
            advance();
            return ResultOk<ast::Literal>(std::make_unique<ast::Fp64Lit>(tok->asFp64()));
        } else if (auto *tok = matches(1, Token::Type::kStrLit)) {
            advance();
            return ResultOk<ast::Literal>(std::make_unique<ast::StrLit>(tok->asString()));
        }

        return {};
    }

    ASTResult_T<ast::TreeNode> Parser::typeOrSymbol()
    {
        if (matches(1, "str")) {
            advance();

            return ResultOk<ast::TreeNode>(std::make_unique<ast::Type>(ast::Type::Kind::kStr));
        } else if (matches(1, "i64")) {
            advance();

            return ResultOk<ast::TreeNode>(std::make_unique<ast::Type>(ast::Type::Kind::kI64));           
        } else if (matches(1, "i32")) {
            advance();

            return ResultOk<ast::TreeNode>(std::make_unique<ast::Type>(ast::Type::Kind::kI32));
        } else if (matches(1, "i16")) {
            advance();

            return ResultOk<ast::TreeNode>(std::make_unique<ast::Type>(ast::Type::Kind::kI16));
        } else if (matches(1, "u64")) {
            advance();

            return ResultOk<ast::TreeNode>(std::make_unique<ast::Type>(ast::Type::Kind::kU64));
        } else if (matches(1, "u32")) {
            advance();

            return ResultOk<ast::TreeNode>(std::make_unique<ast::Type>(ast::Type::Kind::kU32));
        } else if (matches(1, "u16")) {
            advance();

            return ResultOk<ast::TreeNode>(std::make_unique<ast::Type>(ast::Type::Kind::kU16));
        }
        
        auto* token = matches(1, Token::Type::kIdentifier);
        
        advance();

        return ResultOk<ast::TreeNode>(std::make_unique<ast::TSymbol>(token->asString()));        
    }

    ASTResult_T<ast::Attribute> Parser::parseAttribute()
    {
        while (continueParsing()) {
            // TODO: Important to review this later
            if (!matches(0, Token::Type::kAt)) break;

            auto* attr_name = matches(1, Token::Type::kIdentifier);

            if (!attr_name) {
                // TODO: Handle error here
                return ResultFail<ast::Attribute>();
            }

            advance();

            std::unique_ptr<ast::TreeNode> node;

            if (matches(1, Token::Type::kLeftParen)) {
                advance();

                node = parseExpression();

                if (!matches(1, Token::Type::kRightParen)) {
                    // TODO: Handle error here
                    return ResultFail<ast::Attribute>();
                }

                advance();
            }

            auto attr = std::make_unique<ast::Attribute>(attr_name->asString(), std::move(node));

            return ResultOk(std::move(attr));
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

    ASTResult_T<ast::UnaryMinus> Parser::checkUnary()
    {
        if (matches(1, Token::Type::kMinus)) {
            advance();

            return ResultOk(std::make_unique<ast::UnaryMinus>(parsePrimary()));
        }

        return {};
    }

    ASTResult_T<ast::Expr> Parser::checkExpr()
    {
        if (matches(1, Token::Type::kLeftParen)) {
            advance();

            auto expr = parseExpression();

            if (!matches(1, Token::Type::kRightParen)) {
                // Handle error here
                return ResultFail<ast::Expr>();
            }

            advance();

            return ResultOk(std::make_unique<ast::Expr>(std::move(expr)));
        }

        return {};
    }

    ASTResult_T<ast::ComponentDecl> Parser::componentDecl()
    {
        bool is_component_exported = false;

        auto as = attributes();

        if (matches(1, "export")) {
            advance();

            is_component_exported = true;
        }

        if (!matches(1, "component")) return {};

        advance();

        auto decl_name = matches(1, Token::Type::kIdentifier);

        if (!decl_name) {
            // TODO: Add error message here                
            return ResultFail<ast::ComponentDecl>();
        }

        std::vector<std::unique_ptr<ast::TreeNode>> inheritances;

        if (matches(1, Token::Type::kColon)) {
            advance();

            do {
                if (matches(1, Token::Type::kComma)) 
                    advance();

                auto iasl = attributes();

                std::unique_ptr<ast::TreeNode> inheritance = parseExpression();

                for (auto& a : iasl)
                    inheritance->addAttribute(std::move(a));

                inheritances.push_back(std::move(inheritance));
            } while (matches(1, Token::Type::kComma));
        }

        if (!matches(1, Token::Type::kLeftBracket)) {
            // TODO: Handle error here                
            return ResultFail<ast::ComponentDecl>();
        }

        advance();

        std::vector<std::unique_ptr<ast::TreeNode>> members;

        while (!matches(1, Token::Type::kRightBracket)) {
            auto as = attributes();

            auto type = typeOrSymbol();

            bool is_optional = false;

            if (matches(1, Token::Type::kQuestion)) {
                advance();

                is_optional = true;
            }

            auto* field_name = matches(1, Token::Type::kIdentifier);

            if (!field_name) {
                // TODO: Handle error here
                return ResultFail<ast::ComponentDecl>();
            }

            advance();

            std::string la;

            if (matches(1, Token::Type::kArrow)) {
                advance();

                auto prop_name = matches(1, Token::Type::kIdentifier);

                if (!prop_name) {
                    // TODO: Handle error here
                    return ResultFail<ast::ComponentDecl>();
                }

                advance();

                la = prop_name->asString();
            }

            std::unique_ptr<ast::TreeNode> field_default_value;

            if (matches(1, Token::Type::kEqual)) {
                advance();

                field_default_value = parseExpression();
            }

            for (auto& m : members) {
                auto* decl = m->cast<ast::Declaration>();

                if (decl->kind() == ast::Declaration::Kind::kMember) {
                    auto* member = static_cast<ast::MemberDecl*>(decl); 

                    if (field_name->match(member->name())) {
                        // TODO: Handle error here
                        return ResultFail<ast::ComponentDecl>();
                    }
                }
            }

            members.push_back(
                std::make_unique<ast::MemberDecl>(
                    std::move(*type),
                    field_name->asString(),
                    std::move(field_default_value),
                    is_optional,
                    la
                )
            );

            for (auto& a : as) members.back()->addAttribute(std::move(a));
        }

        if (!matches(1, Token::Type::kRightBracket)) {
            // TODO: Handle error here
            return ResultFail<ast::ComponentDecl>();
        }

        advance();

        auto comp_decl = std::make_unique<ast::ComponentDecl>(
            decl_name->asString(),
            std::move(members),
            std::move(inheritances),
            is_component_exported
        );

        for (auto& a : as) comp_decl->addAttribute(std::move(a));

        return ResultOk(std::move(comp_decl));
    }

    ASTResult_T<ast::ImportDecl> Parser::importDecl() {
        if (!matches(1, "import")) return {};
        
        advance();

        bool is_cc = false;
                                                
        std::vector<std::filesystem::path> pathlist;

        if (matches(1, Token::Type::kLeftParen)) {
            advance();

            while (!matches(1, Token::Type::kRightParen)) {
                auto* path = matches(1, Token::Type::kStrLit);

                if (!path) {
                    // TODO: Handle error here
                    return ResultFail<ast::ImportDecl>();
                }

                advance();

                pathlist.push_back(path->asString());
            }

            if (!matches(1, Token::Type::kRightParen)) {
                // TODO: Handle error here.
                return ResultFail<ast::ImportDecl>();
            }

            advance();
        } else {
            const Token* next = matches(1, Token::Type::kStrLit);

            if (!next) {
                // TODO: Handle error here.
                return ResultFail<ast::ImportDecl>();
            }

            advance();

            pathlist.push_back(next->asString());
        }

        return ResultOk(std::make_unique<ast::ImportDecl>(std::move(pathlist), is_cc));
    }

    bool Parser::continueParsing()
    {
        return (m_tokenIdx + 1 < m_lexer->size());
    }

    void Parser::globalDecl()
    {
        sync(Token::Type::kRightBracket, [&]() -> ParseStatus {
            auto cd = componentDecl();

            if (cd.errored)
                return ParseStatus::kError;

            if (cd.matched) {
                // TODO: Add this to AST builder
                cd.unwrap()->print(std::cout, 0);
            
                return ParseStatus::kSuccess;
            }

            auto id = importDecl();

            if (id.errored)
                return ParseStatus::kError;

            if (id.matched) {
                // TODO: Add this to AST builder
                id.unwrap()->print(std::cout, 0);
            
                return ParseStatus::kSuccess;
            }

            return ParseStatus::kContinue;
        });
    }

    void Parser::checkAttribute()
    {
        auto as = attributes();

        if (as.empty()) return;

        auto nxt = componentDecl();

        if (nxt.errored) {
            // TODO: Handle error here
            return;
        }

        if (nxt.matched) {
            for (auto& a : as) nxt.unwrap()->addAttribute(std::move(a));
            return;
        }
    }
    
    std::unique_ptr<ast::TreeNode> Parser::parsePrimary()
    {
        auto tk = advance();

        if (!tk) return nullptr;

        std::unique_ptr<ast::TreeNode> result;

#       ifdef MANA_IS_VERBOSE
            std::visit([](auto&& arg) {
                std::cout << "Processing token: " << arg << std::endl;
            }, tk->value);
#       endif

        while (continueParsing()) {
            globalDecl();

            checkAttribute();
        }

        return result;
    }

    const Token* Parser::matches(int64_t off, Token::Type token_type, bool skip_ws, bool skip_lnbrks)
    {
        auto* token = peek(off, skip_ws, skip_lnbrks);

        if (!token || token->kind != token_type) return nullptr;

        return token;
    }

    const Token* Parser::matches(int64_t off, std::string_view str, bool skip_ws, bool skip_lnbrks)
    {
        auto* token = peek(off, skip_ws, skip_lnbrks);

        if (!token || token->kind != Token::Type::kIdentifier) return nullptr;

        if (token->match(str)) return token;

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