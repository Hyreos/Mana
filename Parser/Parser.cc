#include "Parser.hh"

#include <iostream>
#include <queue>
#include <stack>

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

        m_lexer = std::make_unique<mana::Lexer>(code);

        if (m_lexer->size() > 0)
            if ((*m_lexer)[0].kind == Token::Type::kWS || (*m_lexer)[0].kind == Token::Type::kLnBrk) {
                consume();

                m_tokenIdx--;
            }

        std::vector<std::unique_ptr<TreeNode>> tr_nodes;
        
        while (canPeek(1)) {
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
            case Token::Type::kMinus:
            case Token::Type::kPlus:
                return 1;
            case Token::Type::kAsterisk:
                return 2;
            case Token::Type::kSlash:
            case Token::Type::kMod:
                return 3;
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
                return Associativity::kLeft;
            default:
                return Associativity::kRight;
        }

        mana::unreachable();
    }

    std::unique_ptr<TreeNode> Parser::parseExpression() {
        return parseExpression1(parsePrimary(), 1ull);
    }

    bool Parser::isOperator(const Token& tok)
    {
        switch (tok.kind) {
            case Token::Type::kAsterisk:
            case Token::Type::kSlash:
            case Token::Type::kPlus:
            case Token::Type::kMinus:
                return true;
            default:
                return false;
        }

        mana::unreachable();
    }

    std::unique_ptr<TreeNode> Parser::parseExpression1(
        std::unique_ptr<TreeNode> lhs,
        size_t min_precedence
    )
    {
        if (!canPeek(1)) return lhs;

        auto* lookahead = peek(1);

        if (isOperator(*lookahead)) {
            consume();

            MANA_CHECK_MAYBE_RETURN(lookahead, "Trying to consume end of stream.");

            while (lookahead && isOperator(*lookahead) && getPrecedence(*lookahead) >= min_precedence) {
                auto op = lookahead;

                auto rhs = parsePrimary();

                MANA_CHECK_MAYBE_RETURN(rhs, "Error while parsing token.");

                lookahead = peek(1);

                while (lookahead && (
                    isOperator(*lookahead) 
                    && getPrecedence(*lookahead) >= getPrecedence(*op) 
                    || (getAssociativity(*lookahead) == Associativity::kRight 
                        && getPrecedence(*lookahead) == getPrecedence(*op))
                ))
                {
                    rhs = parseExpression1(
                        std::move(rhs),
                        getPrecedence(*op) + ((getPrecedence(*lookahead) > getPrecedence(*op)) ? 1 : 0) 
                    );

                    lookahead = peek(1);
                }

                switch (op->kind) {
                    case Token::Type::kPlus: {
                        lhs = std::make_unique<SumOp>(std::move(lhs), std::move(rhs));
                    } break;

                    case Token::Type::kMinus: {
                        lhs = std::make_unique<SubOp>(std::move(lhs), std::move(rhs));
                    } break;

                    case Token::Type::kSlash: {
                        lhs = std::make_unique<DivOp>(std::move(lhs), std::move(rhs));
                    } break;

                    case Token::Type::kAsterisk: {
                        lhs = std::make_unique<MulOp>(std::move(lhs), std::move(rhs));
                    } break;
                    default:
                        mana::unreachable();
                }
            }

            return lhs;
        } else {
            return lhs;
        }
    }
    
    std::unique_ptr<TreeNode> Parser::parsePrimary()
    {
        auto tk = consume();

        if (!tk) return nullptr;

        std::unique_ptr<TreeNode> result;

#       ifdef MANA_IS_VERBOSE
            std::visit([](auto&& arg) {
                std::cout << "Processing token: " << arg << std::endl;
            }, tk->value);
#       endif

        switch (tk->kind) {
            case Token::Type::kEOF:
                return nullptr;
            case Token::Type::kAt: {
                const mana::Token* attr_name;
                
                MANA_TRY_GET(consumeCheck (Token::Type::kIdentifier), attr_name, "Missing identifier in attribute.");

                MANA_CHECK_MAYBE_RETURN(attr_name->asStringView() == "serialize", "Invalid identifier in attribute.");
            
                MANA_CHECK_MAYBE_RETURN(consumeCheck (Token::Type::kLeftParen), "Missing '(' in attribute."); // (
                
                auto attr_value = parseExpression();

                MANA_CHECK_MAYBE_RETURN(consumeCheck (Token::Type::kRightParen), "Missing ')' in attribute."); // )

                auto attr = std::make_unique<Attribute>(attr_name->asString(), std::move(attr_value));

                std::unique_ptr<TreeNode> nxt;
                
                MANA_TRY_GET(parsePrimary(), nxt, "Failed to gerante ASTNode");

                nxt->addAttribute(std::move(attr));

                result = std::move(nxt);
            } break;

            case Token::Type::kI16Lit: 
                result = std::make_unique<Int16Lit>(tk->as16Int());
                break;
            case Token::Type::kI32Lit: 
                result = std::make_unique<Int32Lit>(tk->as32Int());
                break;
            case Token::Type::kI64Lit: 
                result = std::make_unique<Int64Lit>(tk->as64Int());
                break;
            case Token::Type::kU16Lit:
                result = std::make_unique<Uint16Lit>(tk->as16UInt());
                break;
            case Token::Type::kU32Lit:
                result = std::make_unique<Uint32Lit>(tk->as32UInt());
                break;
            case Token::Type::kU64Lit:
                result = std::make_unique<Uint64Lit>(tk->as64UInt());
                break;

            case Token::Type::kIdentifier: {
                if (tk->asStringView() == "component") {
                    std::vector<std::unique_ptr<TreeNode>> fields;

                    const Token* identifier;
                    
                    MANA_TRY_GET(consumeCheck (Token::Type::kIdentifier), identifier, "Missing identifier in component declaration.");
                    
                    MANA_CHECK_MAYBE_RETURN(consumeCheck (Token::Type::kLeftBracket), "Missing '{' in component declaration");

                    // Keep looping until next token is a left bracket
                    while (canPeek(1) && peek(1)->kind != Token::Type::kRightBracket) {                        
                        auto field_type = parsePrimary();

                        // Peek a identifier
                        const Token* field_name;
                        
                        MANA_TRY_GET(consumeCheck (Token::Type::kIdentifier), field_name, "Invalid token");

                        fields.push_back(std::make_unique<CField>(std::move(field_type), field_name->asString()));
                    }

                    MANA_CHECK_MAYBE_RETURN(
                        consumeCheck (Token::Type::kRightBracket), 
                        "Missing '}' at end of component declaration."
                    );

                    result = std::make_unique<Component>(identifier->asString(), std::move(fields));
                } else if (tk->asStringView() == "string") result = std::make_unique<TSymbol>("string");
                else result = std::make_unique<TSymbol>(tk->asString());
            } break;
            case Token::Type::kMinus: {
                result = std::make_unique<UnaryMinus>(parsePrimary());
            } break;

            case Token::Type::kLeftParen:
                result = parseExpression();
                MANA_CHECK_MAYBE_RETURN(
                    consumeCheck(Token::Type::kRightParen),
                    "Expected ')' at end of expression group."
                ); // consume )                
                break;
            case Token::Type::kRightParen:
            case Token::Type::kPlus:
            case Token::Type::kAsterisk:
            case Token::Type::kSlash:
            case Token::Type::kDecrement:
                MANA_FATAL_NO_RETURN("Received invalid operator while doing primary parsing.");
            default:
                MANA_FATAL_NO_RETURN("Unrecognized token.");
        }

        return result;
    }

    const Token* Parser::peekExpected(int64_t off, Token::Type kind, bool skip_ws, bool skip_lnbrks)
    {
        auto tk = peek(off, skip_ws, skip_lnbrks);

        if (tk)
            if (tk->kind != kind)
                return nullptr;

        return tk;
    }

    bool Parser::canPeek(int64_t off, bool skip_ws, bool skip_lnbrks) {
        if (off == 0) return true;

        auto tokenidx = m_tokenIdx + 1;

        while (off != 0 && tokenidx < m_lexer->size()) {
            if (skip_ws && (*m_lexer)[tokenidx].kind == Token::Type::kWS) {
                tokenidx++;
                continue;
            }

            if (skip_lnbrks && (*m_lexer)[tokenidx].kind == Token::Type::kLnBrk) {
                tokenidx++;
                continue;
            }

            off--;
            
            if (off > 0) tokenidx++;
        }

        if (off != 0) return false;

        return true;
    }

    const Token* Parser::peek(int64_t off, bool skip_ws, bool skip_lnbrks) 
    {
        if (off == 0) return &(*m_lexer)[m_tokenIdx];

        int64_t tokenidx;

        if (off > 0) {
            if (m_tokenIdx == m_lexer->size() - 1) return nullptr;

            tokenidx = m_tokenIdx + 1;

            for (; off != 0 && tokenidx < m_lexer->size(); tokenidx++) {
                if (skip_ws && (*m_lexer)[tokenidx].kind == Token::Type::kWS) 
                    continue;

                if (skip_lnbrks && (*m_lexer)[tokenidx].kind == Token::Type::kLnBrk)
                    continue;
            
                if (--off == 0) break;
            }
        } else {
            if (m_tokenIdx == 0) return nullptr;

            tokenidx = m_tokenIdx - 1;

            for (; off != 0 && tokenidx >= 0; tokenidx--) {
                if (skip_ws && (*m_lexer)[tokenidx].kind == Token::Type::kWS) 
                    continue;

                if (skip_lnbrks && (*m_lexer)[tokenidx].kind == Token::Type::kLnBrk)
                    continue;
            
                if (++off == 0) break;
            }
        }

        if (off != 0) return nullptr;

        return &(*m_lexer)[tokenidx];   
    }

    const Token* Parser::consumeCheck (Token::Type kind, size_t off, bool skip_ws, bool skip_lnbrks) 
    {
        const Token* tk = consume(off, skip_ws, skip_lnbrks);

        if (tk->kind != kind) {           
            return nullptr;
        }

        return tk;   
    }

    const Token* Parser::consume(size_t off, bool skip_ws, bool skip_lnbrks) 
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