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

        std::vector<std::unique_ptr<ast::TreeNode>> tr_nodes;
        
        doParse();
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
        unreachable();
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
        auto expression = literal();

        return expression;
    }

    Result<const ast::Expression*> Parser::expectPrimaryExpression()
    {
        auto expression = primaryExpression();
    
        if (!expression.matched) return Failure::kError;

        return expression;
    }

    const std::vector<Parser::Error>& Parser::errorList() const
    {
        return m_errors;
    }

    Result<const ast::Expression*> Parser::expression()
    {
        auto result = primaryExpression();

        if (result.matched) {
            auto next = peek(1);

            if (next && isOperator(*next)) {
                result = parseExpression1(result.unwrap(), 0ull);
            }
        }

        return result;
    }

    Result<const ast::Expression*> Parser::expectExpression() 
    {
        auto result = expression();

        if (!result.matched) return Failure::kError;

        return result;
    }

    void Parser::error(const std::string& message)
    {
        m_errors.push_back({ m_stats, message });
    }

    bool Parser::isOperator(const Token& tok)
    {
        switch (tok.kind) {
            case Token::Type::kAsterisk:
            case Token::Type::kSlash:
            case Token::Type::kPlus:
            case Token::Type::kMinus:
            case Token::Type::kDualColon:
                return true;
            default:
                return false;
        }

        unreachable();
    }

    Result<const ast::Expression*> Parser::parseExpression1(
        const ast::Expression* lhs,
        size_t min_precedence
    )
    {
        auto* lookahead = peek(1);

        if (!lookahead) return lhs;

        while (lookahead && isOperator(*lookahead) && getPrecedence(*lookahead) >= min_precedence) {
            auto op = lookahead;

            advance();

            auto rhs_expr = primaryExpression();

            if (rhs_expr.errored || !rhs_expr.matched) {
                error("error while parsing expression.");
                
                return Failure::kError;
            }

            auto* rhs = rhs_expr.unwrap();

            lookahead = peek(1);

            while (lookahead && (
                isOperator(*lookahead) 
                && (getPrecedence(*lookahead) >= getPrecedence(*op) 
                        || (getAssociativity(*lookahead) == Associativity::kRight 
                                && getPrecedence(*lookahead) == getPrecedence(*op)))
            ))
            {
                auto rhs_expr2 = parseExpression1(
                    rhs,
                    getPrecedence(*op) + ((getPrecedence(*lookahead) > getPrecedence(*op)) ? 1 : 0)
                );

                if (rhs_expr2.errored || !rhs_expr2.matched) {
                    error("error while parsing expression.");
                    
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
                    error("invalid operator");
                    
                    return Failure::kError;
            }
        }

        return lhs;
    }

    Result<std::vector<const ast::Expression*>> Parser::expectExpressionList()
    {
        auto expression_list = expressionList();

        if (!expression_list.matched || expression_list.errored) return Failure::kError;

        return expression_list;
    }

    Result<std::vector<const ast::Expression*>> Parser::expressionList()
    {
        std::vector<const ast::Expression*> expression_list;

        for (size_t i = 0; continueParsing(); i++) {
            if (i > 0)
                if (!match(Token::Type::kComma))                   
                    break;

            auto expr = expression();

            if (expr.errored) return Failure::kError;

            if (!expr.matched) break;

            expression_list.push_back(expr.value);
        }

        return expression_list;
    }

    Result<std::vector<const ast::Attribute*>> Parser::attributes()
    {
        std::vector<const ast::Attribute*> attrs;

        while (continueParsing()) {
            auto a = sync(Token::Type::kRightParen, [&]() -> Result<const ast::Attribute*> {
                auto a = attribute();

                if (a.errored) return Failure::kError;

                return a;
            });

            if (!a.matched && !a.errored) break;
            else if (a.matched) attrs.push_back(a.unwrap());
        }

        return attrs;
    }

    Result<const ast::FunctionParameter*> Parser::functionParameter()
    {
        auto attr_list = attributes();

        auto qual_list = qualifiers();

        auto type_identifier = expectIdentifierExpression();

        if (type_identifier.errored) {
            std::cout << "Next token was " << peek(1)->toString() << std::endl;

            error("missing type identifier in function parameter.");

            return Failure::kError;
        }

        auto type = m_ctx.create<ast::Type>(type_identifier.unwrap());

        auto variable_identifier = expectIdentifierExpression();

        if (variable_identifier.errored) return Failure::kError;

        const ast::Expression* default_value_expression;

        if (match(Token::Type::kEqual)) {
            auto expr = expectExpression();

            if (expr.errored) {
                error("missing expression after '=' in function parameter.");
                    
                return Failure::kError;
            }

            default_value_expression = expr.unwrap();
        }

        return m_ctx.create<ast::FunctionParameter>(
            attr_list.unwrap(),
            qual_list.unwrap(),
            type,
            type_identifier.unwrap(),
            default_value_expression
        );
    }

    Result<const std::vector<const ast::FunctionParameter*>> Parser::functionParameterList()
    {
        std::vector<const ast::FunctionParameter*> parameters;

        while (continueParsing() && peek(1)->kind != Token::Type::kRightParen) {
            auto param = functionParameter();

            if (param.errored)     
                return Failure::kError;

            if (param.matched)
                parameters.push_back(param.unwrap());

            if (!match(Token::Type::kComma)) {
                if (peek(1)->kind == Token::Type::kRightParen) 
                    break;
                else {
                    error("missing comma ',' when separating function parameters.");

                    return Failure::kError;
                }
            }
        }

        return parameters;
    }

    Result<const ast::FunctionDeclaration*> Parser::functionDeclaration(
        const std::vector<const ast::Attribute*>& attributes,
        const std::vector<const ast::Qualifier*>& qualifiers
    ) {
        if (match("fn")) {
            auto name = expectIdentifierExpression();

            if (name.errored) {
                error("missing name identifier in function declaration.");

                return Failure::kError;
            }

            if (!match(Token::Type::kLeftParen)) {
                error("missing left parenthesis '(' after name identifier in function declaration.");

                return Failure::kError;
            }

            auto parameters = functionParameterList();

            if (!match(Token::Type::kRightParen)) {
                error("missing right parenthesis ')' after name identifier in function declaration.");

                return Failure::kError;
            }

            const ast::Type* returnType;

            if (match(Token::Type::kArrow)) {
                auto return_type_identifier = expectIdentifierExpression();

                if (return_type_identifier.errored) {
                    error("missing return type identifier after '->' when declaring function.");
                    
                    return Failure::kError;
                }

                returnType = m_ctx.create<ast::Type>(return_type_identifier.unwrap());
            } else {
                returnType = m_ctx.create<ast::Type>(
                    m_ctx.create<ast::IdentifierExpression>("void")
                );
            }

            return m_ctx.create<ast::FunctionDeclaration>(
                returnType,
                name.unwrap(),
                attributes,
                qualifiers,
                parameters.unwrap()
            );
        }

        return Failure::kNoMatch;
    }

    Result<const ast::LiteralExpression*> Parser::literal()
    {
        if (auto* tok = match(Token::Type::kInteger))           
            return m_ctx.create<ast::IntegerLiteralExpression>(
                ast::IntegerLiteralExpression::Sign::kSigned,
                tok->asInteger()
            );
        else if (auto* tok = match(Token::Type::kStrLit))
            return m_ctx.create<ast::StringLiteralExpression>(tok->asStr());
        else if (auto* tok = match("true")) 
            return m_ctx.create<ast::BoolLiteralExpression>(true);
        else if (auto* tok = match("false"))
            return m_ctx.create<ast::BoolLiteralExpression>(false);

        return Failure::kNoMatch;
    }

    Result<const ast::IdentifierExpression*> Parser::expectIdentifierExpression()
    {
        auto identifier = identifierExpression();

        if (!identifier.matched) return Failure::kError;

        return identifier;
    }

    Result<const ast::IdentifierExpression*> Parser::identifierExpression()
    {
        auto* token = match(Token::Type::kIdentifier);

        if (token) return m_ctx.create<ast::IdentifierExpression>(token->asStr());

        return Failure::kNoMatch;
    }

    Result<const ast::Attribute*> Parser::attribute()
    {
        if (match(Token::Type::kAt)) {
            static std::array<std::string_view, 2> kAttributeList = {
                "cc",
                "serialize"
            };

            auto name_token = match(kAttributeList);

            if (!name_token) {
                error("unrecognized attribute '@" + peek(1)->toString() + "'.");
                
                return Failure::kError;
            }

            mana::Result<std::vector<const ast::Expression*>> expr_list;

            if (match(Token::Type::kLeftParen)) {
                expr_list = expressionList();

                if (expr_list.errored) return Failure::kError;

                if (!match(Token::Type::kRightParen)) {
                    error("missing ')' at end of expression list when parsing attribute params.");
                    
                    return Failure::kError;
                }
            } else {
                error("missing '(..)' after '@" + peek(1)->toString() + "'.");

                return Failure::kError;
            }

            if (name_token->match("cc")) {
                if (expr_list.value.size() > 0) {
                    error("builtin attribute '@cc' doesn't accept any parameters.");
                    return Failure::kError;
                }
            
                return m_ctx.create<ast::CCAttribute>();
            } else if (name_token->match("serialize")) {
                if (expr_list.value.size() == 0) {
                    error("builtin attribute '@serialize' needs at least one parameter.");
                    
                    return Failure::kError;
                } else if (expr_list.value.size() != 1) {
                    error("builtin attribute '@serialize' can only accept a single parameter.");
                    
                    return Failure::kError;
                }
                
                return m_ctx.create<ast::SerializeAttribute>(expr_list.value[0]);
            }

            CHECK(false);
        }

        return Failure::kNoMatch;
    }

    Result<std::vector<const ast::Qualifier*>> Parser::qualifiers()
    {
        static std::array<std::string_view, 2> kQualifiers = {{
            "readonly",
            "export"
        }};

        std::vector<const ast::Qualifier*> qualifier_list;

        while (continueParsing()) {
            auto qualifier_name = match(kQualifiers);

            if (qualifier_name) {
                if (qualifier_name->match("readonly"))
                    qualifier_list.push_back(m_ctx.create<ast::ReadonlyQualifier>());
                else if (qualifier_name->match("export"))
                    qualifier_list.push_back(m_ctx.create<ast::ExportQualifier>());
                else error("unimplemented qualifier.");
            } else break;
        }

        if (qualifier_list.size() > 0) return qualifier_list;
        else return Failure::kNoMatch;
    }

    Result<const ast::UnaryExpression*> Parser::unaryExpression()
    {
        if (match(Token::Type::kMinus)) {
        } else if (match(Token::Type::kPlus)) {
        }

        return Failure::kNoMatch;
    }

    Result<const ast::Expression*> Parser::expectGroup()
    {
        if (match(Token::Type::kLeftParen)) {
            auto expr = expectExpression();

            if (!match(Token::Type::kRightParen)) {
                return Failure::kError;
            }

            return std::move(expr);
        }

        return Failure::kNoMatch;
    }

    Result<const ast::LiteralExpression*> Parser::expectLiteral()
    {
        auto lit = literal();

        if (!lit.matched) return Failure::kError;

        return lit;
    }

    Result<const ast::EnumDeclaration*> Parser::enumDeclaration(const std::vector<const ast::Attribute*> _attributes)
    {
        if (match("enum")) {
            auto name = match(Token::Type::kIdentifier);

            if (!name) {
                error("missing name indetifier in enum declaration.");

                return Failure::kError;
            }

            match(Token::Type::kLeftBracket);

            std::vector<const ast::EEntryDeclaration*> entries;

            size_t i = 0;

            while (continueParsing()) {
                if (match(Token::Type::kRightBracket)) break;

                if (i++ > 0) 
                    if (!match(Token::Type::kComma)) {
                        error("missing comma while separating enum entries.");
                        
                        return Failure::kError;
                    }

                auto entry_identifier = match(Token::Type::kIdentifier);
            
                if (!entry_identifier) {
                    error("missing name identifier in enum entry.");

                    return Failure::kError;
                }

                const ast::Expression* assign_expression = nullptr;

                if (match(Token::Type::kEqual)) {
                    auto expr = expectLiteral();

                    if (expr.errored) {
                        error("missing literal in enum entry.");
                        
                        return Failure::kError;
                    }

                    assign_expression = expr.unwrap();
                }

                entries.push_back(
                    m_ctx.create<ast::EEntryDeclaration>(
                        entry_identifier->asStr(),
                        assign_expression,
                        std::vector<const ast::Attribute*>()
                    )
                );
            }

            return m_ctx.create<ast::EnumDeclaration>(
                name->asStr(),
                entries,
                _attributes
            );
        }

        return Failure::kNoMatch;
    }

    Result<const ast::ComponentDeclaration*> Parser::componentDeclaration(
        const std::vector<const ast::Attribute*> attributes_list,
        const std::vector<const ast::Qualifier*> qualifier_list
    )
    {
        if (!match("component")) return Failure::kNoMatch;

        auto decl_name = match(Token::Type::kIdentifier);

        if (!decl_name) {
            error("missing identifier when declaring a component name.");
            return Failure::kError;
        }

        std::vector<const ast::IdentifierExpression*> inheritances;

        if (match(Token::Type::kColon)) {
            do {
                auto inh = identifierExpression();

                if (inh.errored) {
                    error("expected an identifier when parsing inheritances of component.");;
                    
                    return Failure::kError;
                }

                if (inh.matched) 
                    inheritances.push_back(inh.unwrap());
            } while (match(Token::Type::kComma));
        }

        if (!match(Token::Type::kLeftBracket)) {
            error("missing token '{' in component declaration.");

            return Failure::kError;
        }

        std::vector<const ast::Declaration*> members;

        while (!match(Token::Type::kRightBracket)) {
            std::vector<const ast::Attribute*> attrs;
            std::vector<const ast::Qualifier*> quals;

            auto attr_list = attributes();

            if (attr_list.errored) return Failure::kError;

            if (attr_list.matched)
                attrs = attr_list.unwrap();

            auto qual_list = qualifiers();

            if (qual_list.errored) return Failure::kError;

            if (qual_list.matched)
                quals = qual_list.unwrap();

            auto func_decl = functionDeclaration(attrs, quals);

            if (func_decl.matched) {
                members.push_back(func_decl.unwrap());
                
                continue;
            }

            auto type_identifier = expectIdentifierExpression();

            if (type_identifier.errored) {
                error("missing type when declaring a component property.");

                return Failure::kError;
            }

            auto type = m_ctx.create<ast::Type>(type_identifier.unwrap());

            bool is_optional = false;
            
            if (match(Token::Type::kQuestion)) 
                is_optional = true;

            auto* field_name = match(Token::Type::kIdentifier);

            if (!field_name) {
                error("missing name identifier when declaring a component property.");

                return Failure::kError;
            }

            std::string la;

            if (match(Token::Type::kArrow)) {
                auto prop_name = match(Token::Type::kIdentifier);

                if (!prop_name) {
                    error("missing identifier after '->' in cross language alias.");

                    return Failure::kError;
                }

                la = prop_name->asStr();
            }

            const ast::Expression* field_default_value;

            if (!is_optional) {
                if (match(Token::Type::kEqual)) {
                    auto expr = expectExpression();

                    if (expr.errored)
                        return Failure::kError;
                
                    field_default_value = expr.unwrap();
                }
            } else {
                if (!match(Token::Type::kEqual)) {
                    error("missing an '=' in non-optional property from component.");

                    return Failure::kError;
                }

                auto expr = expectExpression();

                if (expr.errored)
                    return Failure::kError;

                field_default_value = expr.unwrap();
            }

            for (auto& m : members) {
                if (m->match<ast::MemberDeclaration>()) {
                    auto* member = static_cast<const ast::MemberDeclaration*>(m); 

                    if (field_name->match(member->name())) {
                        error("error when declaring member, identifier '" 
                            + field_name->asStr() + "' already exists in this component.");

                        return Failure::kError;
                    }
                }
            }

            members.push_back(
                m_ctx.create<ast::MemberDeclaration>(
                    type,
                    field_name->asStr(),
                    std::move(field_default_value),
                    is_optional,
                    attrs,
                    quals
                )
            );
        }

        auto comp_decl = m_ctx.create<ast::ComponentDeclaration>(
            decl_name->asStr(),
            std::move(members),
            std::move(inheritances),
            qualifier_list,
            attributes_list
        );
        
        return comp_decl;
    }

    Result<const ast::ImportDeclaration*> Parser::importDeclaration(const std::vector<const ast::Attribute*> attribute_list) {
        if (!match("import")) return Failure::kNoMatch;
        
        bool is_cc = false;
                                                
        std::vector<std::filesystem::path> pathlist;

        if (match(Token::Type::kLeftParen)) {
            while (!match(Token::Type::kRightParen)) {
                auto* path = match(Token::Type::kStrLit);

                if (!path) {
                    error("missing string literal in an import declaration.");

                    return Failure::kError;
                }

                pathlist.push_back(path->asStr());
            }
        } else {
            const Token* next = match(Token::Type::kStrLit);

            if (!next) {
                error("error when declaring import, missing string literal.");

                return Failure::kError;
            }

            pathlist.push_back(next->asStr());
        }

        return m_ctx.create<ast::ImportDeclaration>(
            std::move(pathlist),
            is_cc,
            attribute_list
        );
    }

    bool Parser::continueParsing()
    {
        return (m_tokenIdx + 1 < m_lexer->size() && !match(Token::Type::kEOF));
    }

    void Parser::globalDeclaration()
    {
        auto attribute_list = attributes();

        auto qualifier_list = qualifiers();

        auto decl = sync(Token::Type::kRightBracket, [&]() -> Result<const ast::Declaration*> {   
            auto cd = componentDeclaration(attribute_list.value, qualifier_list.value);

            if (cd.errored) 
                error("error while parsing a component declaration.");

            return cd;
        });

        if (decl.matched) {    
            decl->print(std::cout, 0);
            return;
        }

        decl = sync(Token::Type::kRightBracket, [&]() -> Result<const ast::Declaration*> {
            auto id = importDeclaration(attribute_list.value);

            if (id.errored) 
                error("error while parsing an import declaration.");

            return id;
        });

        if (decl.matched) {
            decl->print(std::cout, 0);
            return;
        }

        decl = sync(Token::Type::kRightBracket, [&]() -> Result<const ast::Declaration*> {
            auto e = enumDeclaration(attribute_list.value);

            if (e.errored)
                error("error while parsing enum.");

            return e;
        });

        if (decl.matched) {
            decl->print(std::cout, 0);
            return;
        }
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
        }
    }

    const Token* Parser::match(Token::Type token_type, bool skip_ws, bool skip_lnbrks)
    {
        auto* token = peek(1, skip_ws, skip_lnbrks);

        if (token && token->kind == token_type) {
            advance(1, skip_ws, skip_lnbrks);

            return token;
        }

        return nullptr;
    }

    const Token* Parser::match(std::string_view str, bool skip_ws, bool skip_lnbrks)
    {
        auto* token = peek(1, skip_ws, skip_lnbrks);

        if (!token || token->kind != Token::Type::kIdentifier) return nullptr;

        if (token->match(str)) {
            advance(1, skip_ws, skip_lnbrks);

            return token;
        }

        return nullptr;
    }

    const Token* Parser::match(std::span<std::string_view> list, bool skip_ws, bool skip_lnbrks)
    {       
        for (auto& str : list) {
            if (auto tok = match(str, skip_ws, skip_lnbrks))
                return tok;
        }

        return nullptr;
    }

    bool Parser::skipLinebreaks()
    {
        bool r = false;

        while (match(Token::Type::kLnBrk, false, false)) {
            advance(1, false, false);
            r = true;
        }

        return r;
    }

    bool Parser::skipWhitespaces() 
    {
        bool r = false;

        while (match(Token::Type::kWS)) {
            advance(1, false, false);
            r = true;
        }

        return r;
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
            m_stats.columnIndex +=(*m_lexer)[m_tokenIdx].toString().size();

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