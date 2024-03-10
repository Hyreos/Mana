#pragma once

#include <string>
#include <vector>
#include <string_view>

namespace mona {
    enum class Kind {
        kNumber,
        kPlus, // +
        kMinus, // -
        kAsterisk, // *
        kDot, // .
        kIdentifier,
        kLeftParenthesis, // (
        kRightParenthesis, // )
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
        kDiv, // /
        kAt, // @
        kWS, // [ ]
        kLnBrk, // \n
        kEOF, // EOF
        kCount
    };

    inline const char* GetKindName(Kind kind) 
    {
        return "HHHHHHHHH";
    }
    
    struct Token {
        Kind kind;
        std::string_view view;
    };
    
    class GrLexer {
    public:
        GrLexer(const std::string& code);
    
        Token& operator[](size_t idx);

        size_t size() const;
    private:
        bool canPeek(size_t offset);

        char peek(size_t offset);

        char consume();

        bool isNumber(char c) const;

        bool isIdentifier(char c) const;

        bool isIdentifierOrNumber(char c) const;

        size_t m_offset = 0;

        std::string m_code;

        std::vector<Token> m_tokens;
    };
}