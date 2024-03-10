#include "Parser.hh"

#include <iostream>

namespace mona {
    Parser::Parser(const std::string& code) 
        : m_lexer { code },
            m_tokenIdx { -1 },
            m_stats { 
                .lineIndex = 1, 
                .columnIndex = 0
            }
    {
        if (m_lexer.size() > 0)
            if (m_lexer[0].kind == Kind::kWS || m_lexer[0].kind == Kind::kLnBrk) {
                consume();

                m_tokenIdx--;
            }
    }

    void Parser::registerErrorCallback(ErrorCallback_T callback)
    {
        g_errorCallback = callback;
    }

    void Parser::parse() 
    {
        for (DeletedUnique_T<TreeNode> ptr = processToken(); ptr; ptr = processToken()) {}
    }
    
    DeletedUnique_T<TreeNode> Parser::processToken()
    {
        auto tk = consume();

        if (!tk) return nullptr;

        switch (tk->kind) {
            case Kind::kEOF:
                return nullptr;
            case Kind::kAt: {
                const mona::Token* attr_name;
                
                MONA_TRY_GET(consumeCheck(Kind::kIdentifier), attr_name, "Missing identifier in attribute.");

                MONA_CHECK_MAYBE_RETURN(
                    attr_name->view == "serialize", 
                    std::format("Invalid identifier '{}' in attribute.", attr_name->view)
                );
            
                MONA_CHECK_MAYBE_RETURN(consumeCheck(Kind::kRightParenthesis), "Missing '(' in attribute."); // (
                
                auto attr_value = processToken();

                MONA_CHECK_MAYBE_RETURN(consumeCheck(Kind::kLeftParenthesis), "Missing ')' in attribute."); // )

                auto attr = MakeUniquePtr<Attribute>(std::string(attr_name->view), std::move(attr_value));

                DeletedUnique_T<TreeNode> nxt;
                
                MONA_TRY_GET(processToken(), nxt, "Failed to gerante ASTNode");

                nxt->addAttribute(std::move(attr));

                return std::move(nxt);
            } break;

            case Kind::kNumber: {
                return MakeUniquePtr<Number>(std::string(tk->view));
            } break;

            case Kind::kIdentifier: {
                if (tk->view == "component") {
                    std::vector<DeletedUnique_T<TreeNode>> fields;

                    const Token* identifier;
                    
                    MONA_TRY_GET(consumeCheck(Kind::kIdentifier), identifier, "Missing identifier in component declaration.");
                    
                    MONA_CHECK_MAYBE_RETURN(consumeCheck(Kind::kRightBracket), "Missing '{' in component declaration");

                    // Keep looping until next token is a left bracket
                    while (peek(1)->kind != Kind::kLeftBracket) {
                        auto field_type = processToken();

                        // Peek a identifier
                        const Token* field_name;
                        
                        MONA_TRY_GET(consumeCheck(Kind::kIdentifier), field_name, "Invalid token");

                        fields.push_back(MakeUniquePtr<CField>(std::move(field_type), std::string(field_name->view)));
                    }

                    MONA_CHECK_MAYBE_RETURN(
                        consumeCheck(Kind::kLeftBracket), 
                        "Missing '}' at end of component declaration."
                    );

                    return MakeUniquePtr<Component>(std::move(fields));
                } else if (tk->view == "string") return MakeUniquePtr<TSymbol>("string");
                else return MakeUniquePtr<TSymbol>(std::string(tk->view));
            } break;
        }

        return nullptr;
    }

    const Token* Parser::peekExpected(size_t off, Kind kind, bool skip_ws, bool skip_lnbrks)
    {
        auto tk = peek(off, skip_ws, skip_lnbrks);

        if (tk)
            if (tk->kind != kind)
                return nullptr;

        return tk;
    }

    bool Parser::canPeek(size_t off, bool skip_ws, bool skip_lnbrks) {
        if (off == 0) return true;

        auto tokenidx = m_tokenIdx + 1;

        while (off != 0 && tokenidx < m_lexer.size()) {
            if (skip_ws && m_lexer[tokenidx].kind == Kind::kWS) {
                tokenidx++;
                continue;
            }

            if (skip_lnbrks && m_lexer[tokenidx].kind == Kind::kLnBrk) {
                tokenidx++;
                continue;
            }

            off--;
            
            if (off > 0) tokenidx++;
        }

        if (off != 0) return false;

        return true;
    }

    const Token* Parser::peek(size_t off, bool skip_ws, bool skip_lnbrks) 
    {
        if (off == 0) return &m_lexer[m_tokenIdx];

        if (m_tokenIdx == m_lexer.size() - 1) return nullptr;

        auto tokenidx = m_tokenIdx + 1;

        for (; off != 0 && tokenidx < m_lexer.size(); tokenidx++) {
            if (skip_ws && m_lexer[tokenidx].kind == Kind::kWS) 
                continue;

            if (skip_lnbrks && m_lexer[tokenidx].kind == Kind::kLnBrk)
                continue;
        
            if (--off == 0) break;
        }

        if (off != 0) return nullptr;

        return &m_lexer[tokenidx];        
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

        if (m_tokenIdx == m_lexer.size() - 1) 
            return nullptr;

        m_tokenIdx++;

        while (off != 0 && m_tokenIdx < m_lexer.size()) {
            m_stats.columnIndex += m_lexer[m_tokenIdx].view.size();

            if (m_lexer[m_tokenIdx].kind == Kind::kWS)
                if (skip_ws) {
                    m_tokenIdx++;
                    continue;
                }

            if (m_lexer[m_tokenIdx].kind == Kind::kLnBrk) {
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

        return &m_lexer[m_tokenIdx];   
    }
}