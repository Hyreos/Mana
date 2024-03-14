#include "Lexer.hh"

#include <charconv>
#include <limits>
#include <iostream>
#include <sstream>
#include <string>

#include <absl/strings/charconv.h>
#include "Helpers/Macros.hh"

namespace mana {
    Lexer::Lexer(const std::string& _code)
    {
        std::vector<Token> tokens;

        m_code = _code;

        bool found_eof = false;

        while (canPeek(0) && !found_eof) {
            char c = peek(0);
            
            if (isNumber(c)) {
                std::string_view number;

                uint64_t value = 0;

                number = std::string_view(&m_code[m_offset], 1);
                auto start = m_offset;

                advance();

                // 0[x]
                //    ^
                // ___|
                if (c == '0' && (matches(0, 'x') || matches(0, 'X'))) {
                    // advance
                    advance();

                    start = m_offset;
                    number = { &m_code[start], 1 };

                    while (isHexDigit(0)) {
                        number = { &m_code[start], m_offset - start + 1 };
                        advance();
                    }

                    if (start == m_offset) 
                        MANA_FATAL_NO_RETURN("Failed to parse hexadecimal integer. Missing integer part of hex.");

                    auto [_, ec] = std::from_chars(number.data(), number.data() + number.length(), value, 16);
                
                    if (ec != std::errc()) {
                        if (ec == std::errc::invalid_argument) {
                            MANA_FATAL_NO_RETURN("Failed to parse hexadecimal integer. Not a number.");
                        } else if (ec == std::errc::result_out_of_range) {
                            MANA_FATAL_NO_RETURN("Failed to parse hexadecimal integer. Number is larger than an i64.");
                        } else {
                            MANA_FATAL_NO_RETURN("Failed to parse hexadecimal integer.");
                        }
                    }
                } else {
                    while (isNumber(peek(0))) {
                        number = { &m_code[start], m_offset - start + 1 };
                        advance();
                    }

                    // If after constructing the number we have a '.', then it's a fractional
                    if (matches(0, '.')) {
                        advance(); // .

                        while (isNumber(peek(0))) {
                            number = { &m_code[start], m_offset - start + 1 };
                            
                            advance();
                        }   

                        double dvalue;

                        auto [_, ec] = absl::from_chars(number.data(), number.data() + number.length(), dvalue);
                    
                        if (ec != std::errc()) {
                            if (ec == std::errc::invalid_argument) {
                                MANA_FATAL_NO_RETURN("Failed to parse fp64. Not a number.");
                            } else if (ec == std::errc::result_out_of_range) {
                                MANA_FATAL_NO_RETURN("Failed to parse fp64. Number is larger than the maximum limit.");
                            } else {
                                MANA_FATAL_NO_RETURN("Failed to parse fp64.");
                            }
                        }

                        if (matches(0, 'f')) {
                            advance();

                            if (mana::in_range<float>(dvalue)) {
                                tokens.push_back(Token {
                                    .kind = Token::Type::kF32Lit,
                                    .value = dvalue
                                });
                            } else {
                                MANA_FATAL_NO_RETURN("Number is larger than maximum float32 limit");
                            }
                        } else {
                            if (matches(0, 'd')) 
                                advance();

                            tokens.push_back(Token {
                                .kind = Token::Type::kF64Lit,
                                .value = dvalue
                            });
                        }

                        continue;
                    } else {
                        // if it's not a fp value, then parse it to an uint64
                        auto [_, ec] = std::from_chars(number.data(), number.data() + number.length(), value, 10);
                    
                        if (ec != std::errc()) {
                            if (ec == std::errc::invalid_argument) {
                                MANA_FATAL_NO_RETURN("Failed to parse integer. Not a number.");
                            } else if (ec == std::errc::result_out_of_range) {
                                MANA_FATAL_NO_RETURN("Failed to parse integer. Number is larger than the i64 max limit.");
                            } else {
                                MANA_FATAL_NO_RETURN("Failed to parse integer.");
                            }
                        }
                    }
                }

                if (matches(0, 'u')) {
                    advance();

                    if (matches(0, 'l')) {
                        advance();

                        tokens.push_back(Token {
                            .kind = Token::Type::kU64Lit,
                            .value = static_cast<int64_t>(value)
                        });
                    } else if (matches(0, 's')) {
                        advance();

                        if (mana::in_range<uint16_t>(value)) {
                            tokens.push_back(Token {
                                .kind = Token::Type::kU16Lit,
                                .value =  static_cast<int64_t>(value)
                            });
                        } else MANA_FATAL_NO_RETURN("Value overflows u16 limits.");
                    } else {
                        if (mana::in_range<uint32_t>(value)) {
                            tokens.push_back(Token {
                                .kind = Token::Type::kU32Lit,
                                .value =  static_cast<int64_t>(value)
                            });
                        } else MANA_FATAL_NO_RETURN("Value overflows u32 limits.");
                    }
                } else if (matches(0, 'l')) {
                    advance();

                    if (mana::in_range<int64_t>(value)) {
                        tokens.push_back(Token {
                            .kind = Token::Type::kI64Lit,
                            .value =  static_cast<int64_t>(value)
                        });
                    } else {                        
                        MANA_FATAL_NO_RETURN("Value overflows i64 limits."); }
                } else if (matches(0, 's')) {
                    advance();

                    if (mana::in_range<int16_t>(value)) {
                        tokens.push_back(Token {
                            .kind = Token::Type::kI16Lit,
                            .value =  static_cast<int64_t>(value)
                        });
                    } else MANA_FATAL_NO_RETURN("Value overflows unsigned integer limits.");
                } else {
                    if (mana::in_range<int32_t>(value)) {
                        tokens.push_back(Token {
                            .kind = Token::Type::kI32Lit,
                            .value =  static_cast<int64_t>(value)
                        });
                    } else MANA_FATAL_NO_RETURN("Value overflows unsigned integer limits.");
                }

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
                    .kind = Token::Type::kIdentifier,
                    .value = identifier
                });

                continue;
            }

            switch (c) {
                case '=':
                    tokens.push_back(Token {
                        .kind = Token::Type::kEqual,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '?':
                    tokens.push_back(Token {
                        .kind = Token::Type::kQuestion,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '>':
                    tokens.push_back(Token {
                        .kind = Token::Type::kGreaterThan,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '<':
                    tokens.push_back(Token {
                        .kind = Token::Type::kLessThan,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '!':
                    tokens.push_back(Token {
                        .kind = Token::Type::kInv,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '(':
                    tokens.push_back(Token {
                        .kind = Token::Type::kLeftParen,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '%':
                    tokens.push_back(Token {
                        .kind = Token::Type::kMod,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;

                case '@':
                    tokens.push_back(Token {
                        .kind = Token::Type::kAt,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case ':':
                    if (matches(1, ':')) {
                        tokens.push_back(Token {
                            .kind = Token::Type::kDualColon,
                            .value = std::string_view { &m_code[m_offset], 2 }
                        });
                        
                        advance();
                        advance();
                    } else {
                        tokens.push_back(Token {
                            .kind = Token::Type::kColon,
                            .value = std::string_view { &m_code[m_offset], 1 }
                        });

                        advance();
                    }

                    break;
                case '/':
                    if (matches(1, '/')) {
                        advance(); advance(); // '//'

                        while (!matches(0, '\n')) advance();

                        advance(); // \n
                    } else if (matches(1, '*')) {
                        advance(); // /
                        advance(); // *

                        while (!(matches(0, '*') && matches(1, '/'))) 
                            advance();

                        advance(); // *
                        advance(); // /
                    } else {
                        tokens.push_back(Token {
                            .kind = Token::Type::kSlash,
                            .value = std::string_view { &m_code[m_offset], 1 }
                        });
                        advance();
                    }
                    break;
                case ')':
                    tokens.push_back(Token {
                        .kind = Token::Type::kRightParen,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '\'':
                case '"': {
                    advance(); // ignore "

                    size_t start = m_offset;
                    size_t count = 0;

                    while (!matches(0, c) && peek(0) != static_cast<char>(EOF)) {
                        count++;
                        advance();
                    }

                    advance(); // ignore next "

                    tokens.push_back(Token {
                        .kind = Token::Type::kStrLit,
                        .value = std::string_view { &m_code[start], count }
                    });
                } break;
                case '+':
                    tokens.push_back(Token {
                        .kind = Token::Type::kPlus,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '-':
                    if (matches(1, '-')) {
                        tokens.push_back(Token {
                            .kind = Token::Type::kDecrement,
                            .value = std::string_view { &m_code[m_offset], 2 }
                        });

                        advance();
                    } else if (matches(1, '>')) {
                        tokens.push_back(Token {
                            .kind = Token::Type::kArrow,
                            .value = std::string_view { &m_code[m_offset], 2 }
                        });

                        advance();
                    } else {
                        tokens.push_back(Token {
                            .kind = Token::Type::kMinus,
                            .value = std::string_view { &m_code[m_offset], 1 }
                        });
                    }

                    advance();
                    break;
                case '*':
                    tokens.push_back(Token {
                        .kind = Token::Type::kAsterisk,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '.':
                    tokens.push_back(Token {
                        .kind = Token::Type::kDot,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case ',':
                    tokens.push_back(Token {
                        .kind = Token::Type::kComma,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '\r': {                   
                    if (matches(1, '\n')) {
                        advance();

                        tokens.push_back(Token {
                            .kind = Token::Type::kLnBrk,
                            .value = std::string_view { &m_code[m_offset], 2 }
                        });
                    }

                    advance(); // discard \r
                    
                    break;
                }
                case '\n':
                    tokens.push_back(Token {
                        .kind = Token::Type::kLnBrk,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '{':
                    tokens.push_back(Token {
                        .kind = Token::Type::kLeftBracket,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '}':
                    tokens.push_back(Token {
                        .kind = Token::Type::kRightBracket,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });
                    advance();
                    break;
                case '\0':
                case EOF:
                    tokens.push_back(Token {
                        .kind = Token::Type::kEOF,
                        .value = std::string_view { &m_code[m_offset], 1 }
                    });

                    advance();

                    found_eof = true;
                    break;
                case '\t':
                case ' ':
                    tokens.push_back(Token {
                        .kind = Token::Type::kWS,
                        .value = std::string_view { &m_code[m_offset], 1 }
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
                .kind = Token::Type::kEOF,
                .value = std::string_view("\0")
            });
        }

        m_tokens = std::move(tokens);

#       ifdef MANA_IS_VERBOSE
            std::cout << "[!] Printing lexer tokens: " << std::endl;
            
            for (auto i = 0; i < m_tokens.size(); i++) {
                std::visit([i](auto&& arg) {
                    using T = std::decay_t<decltype(arg)>;

                    if (i > 0) std::cout << ", ";

                    std::cout << "\"";
                    
                    if constexpr (std::is_same_v<T, std::string_view>) {
                        for (auto c : arg) {
                            auto unesc = unescapeCharacter(c);

                            if (unesc != "") std::cout << unesc;
                            else std::cout << c;
                        }
                    } else {
                        std::cout << arg;
                    }

                    std::cout << "\"";
                }, m_tokens[i].value);
            }
            
            std::cout << std::endl;
#       endif
    }

    bool Lexer::matches(size_t offset, char c)
    {
        return (canPeek(offset) && peek(offset) == c);
    }

    bool Lexer::isHexDigit(size_t offset)
    {
        if (!canPeek(1)) return false;

        auto c = peek(offset);

        return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
    }

    char Lexer::peek(size_t off)
    {
        if (!canPeek(off)) return static_cast<char>(EOF);

        return m_code[m_offset + off];
    }

    size_t Lexer::size() const
    {
        return m_tokens.size();
    }

    bool Lexer::canPeek(size_t off)
    {
        return (m_offset + off < m_code.size());
    }

    char Lexer::advance()
    {
        return m_code[m_offset++];
    }

    bool Lexer::isIdentifier(char c) const
    {
        return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
    }

    bool Lexer::isNumber(char c) const
    {
        return (c >= '0' && c <= '9');
    }

    bool Lexer::isIdentifierOrNumber(char c) const
    {
        return (isNumber(c) || isIdentifier(c));
    }

    Token& Lexer::operator[](size_t idx)
    {
        CHECK(idx < m_tokens.size());

        return m_tokens[idx];
    }
}