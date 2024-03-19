#pragma once

#include "AST/CloneContext.hh"

#include "AST/Attribute.hh"
#include "AST/BinaryExpression.hh"
#include "AST/BoolLiteralExpression.hh"
#include "AST/ComponentDeclaration.hh"
#include "AST/Declaration.hh"
#include "AST/Expression.hh"
#include "AST/FloatLiteralExpression.hh"
#include "AST/IdentifierExpression.hh"
#include "AST/ImportDeclaration.hh"
#include "AST/IntegerLiteralExpression.hh"
#include "AST/LiteralExpression.hh"
#include "AST/MemberDeclaration.hh"
#include "AST/Qualifier.hh"
#include "AST/StringLiteralExpression.hh"
#include "AST/TreeNode.hh"
#include "AST/TypeDeclaration.hh"
#include "AST/UnaryExpression.hh"
#include "AST/CCAttribute.hh"
#include "AST/SerializeAttribute.hh"
#include "AST/ExportQualifier.hh"
#include "AST/ReadonlyQualifier.hh"

#include "Helpers/Deleted.hh"
#include "Helpers/Macros.hh"

#include <array>
#include <queue>
#include <stack>
#include <span>

#include "Lexer.hh"

namespace mana {
    enum class Failure {
        kNoMatch,
        kError
    };

    template<typename T>
    struct Result {
        Result(Failure failure)
        {
            if (failure == Failure::kNoMatch) {
                errored = false;
                matched = false;
            } else {
                errored = true;
                matched = false;
            }
        }

        template<typename U>
        Result(Result<U>&& rhs) {
            value = std::move(rhs.value);
            matched = rhs.matched;
            errored = rhs.errored;
        }

        Result() : 
            errored{ false }, 
            matched { false }
        {
        }
        
        Result(T ptr) : 
            errored { false }, 
            matched { true }, 
            value { ptr } 
        {
        }

        Result(bool _erroed, bool _matched) : 
            errored { _erroed }, 
            matched { _matched }
        {
        }

        Result(bool _erroed, bool _matched, T value) : 
            errored { _erroed }, 
            matched { _matched }, 
            value { std::move(value) }
        {
        }

        T&& unwrap() 
        {
            return std::move(value);
        }

        T& operator->()
        {
            return value;
        }

        bool ok() {
            return !errored;
        }

        bool errored;
        bool matched;
        T value;
    };

    template<typename T>
    Result<T> Match(std::unique_ptr<T>&& value)
    {
        return Result<T>(false, true, std::move(value));
    }

    class Parser {
    public:
        struct Stats {
            size_t lineIndex, 
            columnIndex;
        };

        struct Error {
            Stats stats;
            std::string message;
        };

        Parser();

        void parse(const std::string& code);

        void registerErrorCallback(ErrorCallback_T callback);

        const std::vector<Error>& errorList() const;
    private:
        enum class Associativity {
            kLeft,
            kRight
        };

        void error(const std::string& message);

        void doParse();

        Result<const ast::Expression*> expectExpression();

        Result<const ast::Expression*> expectGroup();

        Result<const ast::Expression*> expression();

        Result<const ast::Expression*> primaryExpression();

        Result<const ast::Expression*> expectPrimaryExpression();

        Result<const ast::Attribute*> attribute();

        Result<std::vector<const ast::Attribute*>> attributes();

        Result<std::vector<const ast::Qualifier*>> qualifiers();

        Result<std::vector<const ast::Expression*>> expectExpressionList();

        Result<std::vector<const ast::Expression*>> expressionList();

        Result<const ast::IdentifierExpression*> expectIdentifierExpression();

        Result<const ast::IdentifierExpression*> identifierExpression();

        Result<const ast::LiteralExpression*> literal();

        Result<const ast::UnaryExpression*> unaryExpression();

        Result<const ast::ComponentDeclaration*> componentDeclaration(const std::vector<const ast::Attribute*> attributes);

        Result<const ast::ImportDeclaration*> importDeclaration(const std::vector<const ast::Attribute*> attributes);

        Result<const ast::Expression*> parseExpression1(const ast::Expression* lhs, size_t min_precedence);

        void globalDeclaration();

        template<class Fn, typename... Args>
        auto sync(Token::Type sync_token, Fn&& f, Args&&... args)
        {
            auto r = f(std::forward<Args>(args)...);

            uint64_t parenCount { 0ull }, bracketCount { 0ull };

            if (!r.ok())
                while (continueParsing()) {
                    if (peek(0)->kind == Token::Type::kLeftParen)
                        parenCount++;
                    else if (peek(0)->kind == Token::Type::kLeftBracket)
                        bracketCount++;
                    else if (peek(0)->kind == Token::Type::kRightParen) {
                        if (parenCount > 0) parenCount--;
                        
                        if (parenCount == 0 && sync_token == Token::Type::kRightParen) break;
                    } else if (peek(0)->kind == Token::Type::kRightBracket) {
                        if (bracketCount > 0) bracketCount--;

                        if (bracketCount == 0 && sync_token == Token::Type::kRightBracket) break;
                    } else if (peek(0)->kind == sync_token) 
                        break;
                    
                    advance();
                }

            return r;
        }

        bool continueParsing();

        bool isOperator(const Token& tok);

        Associativity getAssociativity(const Token& tk);

        size_t getPrecedence(const Token& tk);

        const Token* peek(uint64_t off, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* match(std::string_view str, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* match(Token::Type token_type, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* match(std::span<std::string_view> list, bool skip_ws = true, bool skip_lnbrks = true);

        bool skipLinebreaks();

        bool skipWhitespaces();

        const Token* advance(size_t off = 1, bool skip_ws = true, bool skip_lnbrks = true);

        std::vector<Error> m_errors;

        Stats m_stats;
        
        int64_t m_tokenIdx;   

        ast::CloneContext m_ctx;

        std::unique_ptr<Lexer> m_lexer;
    };
}