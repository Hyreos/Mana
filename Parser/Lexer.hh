#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <variant>

#include "Helpers/Macros.hh"

namespace mana {    
    struct Token {
        enum class Type {
            kI32Lit,
            kU32Lit,
            
            kI64Lit,
            kU64Lit,
            
            kI16Lit,
            kU16Lit,
            
            kF32Lit,
            kF64Lit,

            kPlus, // +
            kMinus, // -
            kAsterisk, // *
            kDot, // .
            kIdentifier,
            kRightParen, // (
            kLeftParen, // )
            kEqual, // =
            kGreaterThan, // >
            kLessThan, // <
            kSingleQuote, // '
            kDoubleQuote, // "
            kComma, // ,
            kInv, // !
            kLeftBracket, // {
            kRightBracket, // }
            kMod, // %
            kSlash, // /
            kAt, // @
            kWS, // [ ]
            kLnBrk, // \n
            kEOF, // EOF
            kCount
        };

        Type kind;

        std::variant<std::string_view, double, int64_t> value;

        inline const std::string_view& asStr() const
        {
            CHECK(kind == Type::kIdentifier);
            
            return std::get<std::string_view>(value);
        }

        inline int16_t as16Int() const {
            CHECK(kind == Type::kI16Lit);

            return static_cast<int16_t>(std::get<int64_t>(value));
        }

        inline int32_t as32Int() const {
            CHECK(kind == Type::kI32Lit);

            return static_cast<int32_t>(std::get<int64_t>(value));
        }

        inline int64_t as64Int() const {
            //CHECK(kind == Type::kI64Lit);

            return std::get<int64_t>(value);
        }

        inline uint16_t as16UInt() const {
            CHECK(kind == Type::kU16Lit);

            return static_cast<uint16_t>(std::get<int64_t>(value));
        }

        inline uint32_t as32UInt() const {
            CHECK(kind == Type::kU32Lit);

            return static_cast<uint32_t>(std::get<int64_t>(value));
        }

        inline uint64_t as64UInt() const {
            CHECK(kind == Type::kU64Lit);

            return static_cast<uint64_t>(std::get<int64_t>(value));
        }
    };
    
    class Lexer {
    public:
        Lexer(const std::string& code);
    
        Token& operator[](size_t idx);

        size_t size() const;
    private:
        bool matches(size_t offset, char c);

        bool isHexDigit(size_t offset);

        bool canPeek(size_t offset);

        char peek(size_t offset);

        char advance();

        bool isNumber(char c) const;

        bool isIdentifier(char c) const;

        bool isIdentifierOrNumber(char c) const;

        size_t m_offset = 0;

        std::string m_code;

        std::vector<Token> m_tokens;
    };
}