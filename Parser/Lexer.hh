#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <sstream>
#include <span>
#include <variant>

#include "Helpers/Macros.hh"

namespace mana {    
    struct Token {
        enum class Type {
            kInteger,
            kFloat,

            kStrLit, // " / '

            kColon, // :

            kArrow, // ->

            kDualColon, // ::

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
            kComma, // ,
            kInv, // !
            kOr, // |
            kLeftBracket, // {
            kRightBracket, // }
            kMod, // %
            kSlash, // /
            kAt, // @
            kWS, // [ ]
            kLnBrk, // \n
            kDecrement, // --
            kSemicolon, // ;
            kQuestion, // ?
            kEOF, // EOF
            kCount
        };

        Type kind;

        std::variant<std::string_view, double, int64_t> value;

        inline std::string toString() const {
            std::string result;
            
            std::visit([&result](auto v) {
                std::stringstream ss;
                ss << v;
                result = ss.str();
            }, value);

            return result;
        }

        inline const std::string asStr() const
        {
            return std::string(asStrView());
        }

        inline const std::string_view& asStrView() const
        {
            CHECK(kind == Type::kIdentifier || kind == Type::kStrLit);
            
            return std::get<std::string_view>(value);
        }

        inline int64_t asInteger() const {
            return std::get<int64_t>(value);
        }

        inline double asDouble() const {
            return std::get<double>(value);
        }

        inline bool match(const char* str) const {
            CHECK(kind == Type::kIdentifier);

            return std::get<std::string_view>(value) == str;
        }

        inline bool match(const std::string& str) const {
            CHECK(kind == Type::kIdentifier);

            return std::get<std::string_view>(value) == str;
        }

        inline bool match(const std::string_view& str) const {
            CHECK(kind == Type::kIdentifier);

            return std::get<std::string_view>(value) == str;
        }

        inline bool match(std::span<std::string_view> array) const
        {
            for (auto& str : array)
                if (match(str))
                    return true;

            return false;
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