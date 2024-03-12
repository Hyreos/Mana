#include "Parser.hh"

#include <iostream>
#include <queue>
#include <stack>

namespace mona {
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

        m_lexer = std::make_unique<mona::GrLexer>(code);

        if (m_lexer->size() > 0)
            if ((*m_lexer)[0].kind == Kind::kWS || (*m_lexer)[0].kind == Kind::kLnBrk) {
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
            case Kind::kMinus:
            case Kind::kPlus:
                return 1;
            case Kind::kAsterisk:
                return 2;
            case Kind::kSlash:
            case Kind::kMod:
                return 3;
            default:
                return 0;
        }
    }

    Parser::Associativity Parser::getAssociativity(const Token& tk) {
        switch (tk.kind) {
            case Kind::kMinus:
            case Kind::kPlus:
            case Kind::kAsterisk:
            case Kind::kSlash:
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
            case Kind::kAsterisk:
            case Kind::kSlash:
            case Kind::kPlus:
            case Kind::kMinus:
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
                    case Kind::kPlus: {
                        lhs = std::make_unique<SumOp>(std::move(lhs), std::move(rhs));
                    } break;

                    case Kind::kMinus: {
                        lhs = std::make_unique<SubOp>(std::move(lhs), std::move(rhs));
                    } break;

                    case Kind::kSlash: {
                        lhs = std::make_unique<DivOp>(std::move(lhs), std::move(rhs));
                    } break;

                    case Kind::kAsterisk: {
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

        switch (tk->kind) {
            case Kind::kEOF:
                return nullptr;
            case Kind::kAt: {
                const mona::Token* attr_name;
                
                MONA_TRY_GET(consumeCheck(Kind::kIdentifier), attr_name, "Missing identifier in attribute.");

                MANA_CHECK_MAYBE_RETURN(
                    attr_name->view == "serialize", 
                    std::format("Invalid identifier '{}' in attribute.", attr_name->view)
                );
            
                MANA_CHECK_MAYBE_RETURN(consumeCheck(Kind::kLeftParen), "Missing '(' in attribute."); // (
                
                auto attr_value = parseExpression();

                MANA_CHECK_MAYBE_RETURN(consumeCheck(Kind::kRightParen), "Missing ')' in attribute."); // )

                auto attr = std::make_unique<Attribute>(std::string(attr_name->view), std::move(attr_value));

                std::unique_ptr<TreeNode> nxt;
                
                MONA_TRY_GET(parsePrimary(), nxt, "Failed to gerante ASTNode");

                nxt->addAttribute(std::move(attr));

                result = std::move(nxt);
            } break;

            case Kind::kNumber: {
                return std::make_unique<Number>(std::string(tk->view));
            } break;

            case Kind::kIdentifier: {
                if (tk->view == "component") {
                    std::vector<std::unique_ptr<TreeNode>> fields;

                    const Token* identifier;
                    
                    MONA_TRY_GET(consumeCheck(Kind::kIdentifier), identifier, "Missing identifier in component declaration.");
                    
                    MANA_CHECK_MAYBE_RETURN(consumeCheck(Kind::kLeftBracket), "Missing '{' in component declaration");

                    // Keep looping until next token is a left bracket
                    while (canPeek(1) && peek(1)->kind != Kind::kRightBracket) {                        
                        auto field_type = parsePrimary();

                        // Peek a identifier
                        const Token* field_name;
                        
                        MONA_TRY_GET(consumeCheck(Kind::kIdentifier), field_name, "Invalid token");

                        fields.push_back(std::make_unique<CField>(std::move(field_type), std::string(field_name->view)));
                    }

                    MANA_CHECK_MAYBE_RETURN(
                        consumeCheck(Kind::kRightBracket), 
                        "Missing '}' at end of component declaration."
                    );

                    result = std::make_unique<Component>(std::string(identifier->view), std::move(fields));
                } else if (tk->view == "string") result = std::make_unique<TSymbol>("string");
                else result = std::make_unique<TSymbol>(std::string(tk->view));
            } break;
            case Kind::kAsterisk:
            case Kind::kMinus:
            case Kind::kSlash:
            case Kind::kPlus:
            case Kind::kLeftParen:
            case Kind::kRightParen:
                MANA_FATAL_NO_RETURN("Received operator while doing primary parsing.");
            default:
                MANA_FATAL_NO_RETURN("Invalid token.");
        }

        return result;
    }

    const Token* Parser::peekExpected(int64_t off, Kind kind, bool skip_ws, bool skip_lnbrks)
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
            if (skip_ws && (*m_lexer)[tokenidx].kind == Kind::kWS) {
                tokenidx++;
                continue;
            }

            if (skip_lnbrks && (*m_lexer)[tokenidx].kind == Kind::kLnBrk) {
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
                if (skip_ws && (*m_lexer)[tokenidx].kind == Kind::kWS) 
                    continue;

                if (skip_lnbrks && (*m_lexer)[tokenidx].kind == Kind::kLnBrk)
                    continue;
            
                if (--off == 0) break;
            }
        } else {
            if (m_tokenIdx == 0) return nullptr;

            tokenidx = m_tokenIdx - 1;

            for (; off != 0 && tokenidx >= 0; tokenidx--) {
                if (skip_ws && (*m_lexer)[tokenidx].kind == Kind::kWS) 
                    continue;

                if (skip_lnbrks && (*m_lexer)[tokenidx].kind == Kind::kLnBrk)
                    continue;
            
                if (++off == 0) break;
            }
        }

        if (off != 0) return nullptr;

        return &(*m_lexer)[tokenidx];   
    }

    const Token* Parser::consumeCheck(Kind kind, size_t off, bool skip_ws, bool skip_lnbrks) 
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
            m_stats.columnIndex += (*m_lexer)[m_tokenIdx].view.size();

            if ((*m_lexer)[m_tokenIdx].kind == Kind::kWS)
                if (skip_ws) {
                    m_tokenIdx++;
                    continue;
                }

            if ((*m_lexer)[m_tokenIdx].kind == Kind::kLnBrk) {
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