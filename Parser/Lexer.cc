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

                advance();

                // 0[b|x|h]
                //     ^
                // ____|_
                if (matches(0, 'b') || matches(0, 'x') || matches(0, 'h')) {
                    // advance
                    advance();

                    // Next token should be a number?????????
                    // Loop until the number is exausted
                    while (isNumber(peek(0))) {
                        number = { &m_code[start], m_offset - start + 1 };
                        
                        advance();
                    }
                } else {
                    // Next token should be a number?????????
                    // Loop until the number is exausted
                    while (isNumber(peek(0))) {
                        number = { &m_code[start], m_offset - start + 1 };
                        
                        advance();
                    }

                    // If after constructing the number we have a '.', then it's a fractional
                    if (matches(0, '.')) {
                        advance(); // .

                        // Next token should be a number?????????
                        // Loop until the number is exausted
                        while (isNumber(peek(0))) {
                            number = { &m_code[start], m_offset - start + 1 };
                            
                            advance();
                        }   
                    }
                }

                if (matches(0, 'u')) {
                    number = { &m_code[start], m_offset - start + 1 };

                    advance();
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
                    
                    advance();
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
                    advance();
                    break;
                case '>':
                    tokens.push_back(Token {
                        .kind = Kind::kGreaterThan,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '<':
                    tokens.push_back(Token {
                        .kind = Kind::kLessThan,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '!':
                    tokens.push_back(Token {
                        .kind = Kind::kInv,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '(':
                    tokens.push_back(Token {
                        .kind = Kind::kRightParenthesis,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '%':
                    tokens.push_back(Token {
                        .kind = Kind::kMod,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;

                case '@':
                    tokens.push_back(Token {
                        .kind = Kind::kAt,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '/':
                    if (matches(1, '*')) {
                        advance(); // /
                        advance(); // *

                        while (!matches(0, '*') && !matches(1, '/')) 
                            advance();

                        advance(); // *
                        advance(); // /
                    } else {
                        tokens.push_back(Token {
                            .kind = Kind::kSlash,
                            .view = { &m_code[m_offset], 1 }
                        });
                        advance();
                    }
                    break;
                case ')':
                    tokens.push_back(Token {
                        .kind = Kind::kLeftParenthesis,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '"':
                    tokens.push_back(Token {
                        .kind = Kind::kDoubleQuote,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '\'':
                    tokens.push_back(Token {
                        .kind = Kind::kSingleQuote,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '+':
                    tokens.push_back(Token {
                        .kind = Kind::kPlus,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '-':
                    tokens.push_back(Token {
                        .kind = Kind::kMinus,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '*':
                    tokens.push_back(Token {
                        .kind = Kind::kAsterisk,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '.':
                    tokens.push_back(Token {
                        .kind = Kind::kDot,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case ',':
                    tokens.push_back(Token {
                        .kind = Kind::kComma,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '\n':
                    tokens.push_back(Token {
                        .kind = Kind::kLnBrk,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '{':
                    tokens.push_back(Token {
                        .kind = Kind::kRightBracket,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '}':
                    tokens.push_back(Token {
                        .kind = Kind::kLeftBracket,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '\0':
                case EOF:
                    tokens.push_back(Token {
                        .kind = Kind::kEOF,
                        .view = { &m_code[m_offset], 1 }
                    });

                    advance();

                    found_eof = true;
                    break;
                case '\t':
                case ' ':
                    tokens.push_back(Token {
                        .kind = Kind::kWS,
                        .view = { &m_code[m_offset], 1 }
                    });
                    advance();
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

    bool GrLexer::matches(size_t offset, char c)
    {
        return (canPeek(offset) && peek(offset) == c);
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

    char GrLexer::advance()
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