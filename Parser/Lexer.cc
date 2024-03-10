#include "Lexer.hh"

#include <iostream>

#include "Helpers/Macros.hh"

namespace mona {
    GrLexer::GrLexer(const std::string& _code)
    {
        std::vector<Token> tokens;

        m_code = _code;

        bool found_eof = false;

        while (canPeek(0) && !found_eof) {
            char c = peek(0);
            
            if (isNumber(c)) {
                std::string_view number;

                number = std::string_view(&m_code[m_offset], 1);
                auto start = m_offset;

                consume();

                // 0[b|x|h]
                //     ^
                // ____|_
                if (peek(0) == 'b' || peek(0) == 'x' || peek(0) == 'h') {
                    // consume
                    consume();

                    // Next token should be a number?????????
                    // Loop until the number is exausted
                    while (isNumber(peek(0))) {
                        number = { &m_code[start], m_offset - start + 1 };
                        
                        consume();
                    }
                } else {
                    // Next token should be a number?????????
                    // Loop until the number is exausted
                    while (isNumber(peek(0))) {
                        number = { &m_code[start], m_offset - start + 1 };
                        
                        consume();
                    }

                    // If after constructing the number we have a '.', then it's a fractional
                    if (peek(0) == '.') {
                        consume(); // .

                        // Next token should be a number?????????
                        // Loop until the number is exausted
                        while (isNumber(peek(0))) {
                            number = { &m_code[start], m_offset - start + 1 };
                            
                            consume();
                        }   
                    }
                }

                if (peek(0) == 'u') {
                    number = { &m_code[start], m_offset - start + 1 };

                    consume();
                }

                tokens.push_back(Token {
                    .kind = Kind::kNumber,
                    .view = number
                });

                continue;
            }

            if (isIdentifier(c)) {
                std::string_view identifier;

                auto start = m_offset;

                while (isIdentifierOrNumber(peek(0))) {
                    identifier = { &m_code[start], m_offset - start + 1 };
                    
                    consume();
                }

                tokens.push_back(Token {
                    .kind = Kind::kIdentifier,
                    .view = identifier
                });

                continue;
            }

            switch (c) {
                case '=':
                    tokens.push_back(Token {
                        .kind = Kind::kEqual,
                        .view = { &m_code[m_offset], 1 }
                    });
                    consume();
                    break;
                case '>':
                    tokens.push_back(Token {
                        .kind = Kind::kGreaterThan,
                        .view = { &m_code[m_offset], 1 }
                    });
                    consume();
                    break;
                case '<':
                    tokens.push_back(Token {
                        .kind = Kind::kLessThan,
                        .view = { &m_code[m_offset], 1 }
                    });
                    consume();
                    break;
                case '!':
                    tokens.push_back(Token {
                        .kind = Kind::kInv,
                        .view = { &m_code[m_offset], 1 }
                    });
                    consume();
                    break;
                case '(':
                    tokens.push_back(Token {
                        .kind = Kind::kRightParenthesis,
                        .view = std::string_view(&m_code[m_offset], 1)
                    });
                    consume();
                    break;
                case '%':
                    tokens.push_back(Token {
                        .kind = Kind::kMod,
                        .view = std::string_view(&m_code[m_offset], 1)
                    });
                    consume();
                    break;

                case '@':
                    tokens.push_back(Token {
                        .kind = Kind::kAt,
                        .view = { &m_code[m_offset], 1 }
                    });
                    consume();

                    break;
                case '/':
                    consume();

                    if (peek(0) == '*') {
                        consume();

                        while (peek(0) != '*' || canPeek(1) && peek(1) != '/') 
                            consume();

                        consume(); // *
                        consume(); // /
                    } else {
                        tokens.push_back(Token {
                            .kind = Kind::kDiv,
                            .view = std::string_view(&m_code[m_offset], 1)
                        });
                    }
                    break;
                case ')':
                    tokens.push_back(Token {
                        .kind = Kind::kLeftParenthesis,
                        .view = std::string_view(&m_code[m_offset], 1)
                    });
                    consume();
                    break;
                case '"':
                    tokens.push_back(Token {
                        .kind = Kind::kDoubleQuote,
                        .view = std::string_view(&m_code[m_offset], 1)
                    });
                    consume();
                    break;
                case '\'':
                    tokens.push_back(Token {
                        .kind = Kind::kSingleQuote,
                        .view = std::string_view(&m_code[m_offset], 1)
                    });
                    consume();
                    break;
                case '+':
                    tokens.push_back(Token {
                        .kind = Kind::kPlus,
                        .view = std::string_view(&m_code[m_offset], 1)
                    });
                    consume();
                    break;
                case '-':
                    tokens.push_back(Token {
                        .kind = Kind::kMinus,
                        .view = std::string_view(&m_code[m_offset], 1)
                    });
                    consume();
                    break;
                case '*':
                    tokens.push_back(Token {
                        .kind = Kind::kAsterisk,
                        .view = std::string_view(&m_code[m_offset], 1)
                    });
                    consume();
                    break;
                case '.':
                    tokens.push_back(Token {
                        .kind = Kind::kDot,
                        .view = std::string_view(&m_code[m_offset], 1)
                    });
                    consume();
                    break;
                case ',':
                    tokens.push_back(Token {
                        .kind = Kind::kComma,
                        .view = std::string_view(&m_code[m_offset], 1)
                    });
                    consume();
                    break;
                case '\n':
                    tokens.push_back(Token {
                        .kind = Kind::kLnBrk,
                        .view = std::string_view(&m_code[m_offset], 1)
                    });
                    consume();
                    break;
                case '{':
                    tokens.push_back(Token {
                        .kind = Kind::kRightBracket,
                        .view = { &m_code[m_offset], 1 }
                    });
                    consume();
                    break;
                case '}':
                    tokens.push_back(Token {
                        .kind = Kind::kLeftBracket,
                        .view = { &m_code[m_offset], 1 }
                    });
                    consume();
                    break;
                case '\0':
                case EOF:
                    tokens.push_back(Token {
                        .kind = Kind::kEOF,
                        .view = std::string_view(&m_code[m_offset], 1)
                    });

                    consume();

                    found_eof = true;
                    
                    break;
                case '\t':
                case ' ':
                    tokens.push_back(Token {
                        .kind = Kind::kWS,
                        .view = std::string_view(&m_code[m_offset], 1)
                    });
                    consume();
                    break;
                default:
                    std::cerr << "ERROR: Unhandled token '" << peek(0) << "'" << std::endl;
                    
                    std::exit(1);
                    
                    return;
            }
        }

        // Add dummy EOF token if not found
        if (!found_eof) {
            m_tokens.push_back(Token {
                .kind = Kind::kEOF,
                .view = std::string_view("\0")
            });
        }

        m_tokens = std::move(tokens);
    }

    char GrLexer::peek(size_t off)
    {
        return m_code[m_offset + off];
    }

    size_t GrLexer::size() const
    {
        return m_tokens.size();
    }

    bool GrLexer::canPeek(size_t off)
    {
        return (m_offset + off < m_code.size());
    }

    char GrLexer::consume()
    {
        return m_code[m_offset++];
    }

    bool GrLexer::isIdentifier(char c) const
    {
        return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
    }

    bool GrLexer::isNumber(char c) const
    {
        return (c >= '0' && c <= '9');
    }

    bool GrLexer::isIdentifierOrNumber(char c) const
    {
        return (isNumber(c) || isIdentifier(c));
    }

    Token& GrLexer::operator[](size_t idx)
    {
        CHECK(idx < m_tokens.size());

        return m_tokens[idx];
    }
}