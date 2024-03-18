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

#include "Helpers/Deleted.hh"
#include "Helpers/Macros.hh"

#include <queue>
#include <stack>

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
        Parser();

        void parse(const std::string& code);

        void registerErrorCallback(ErrorCallback_T callback);
    private:
        enum class Associativity {
            kLeft,
            kRight
        };

        void doParse();

        Result<const ast::Expression*> expectExpression(bool isExpr = false);

        Result<const ast::Expression*> expectGroup(bool isExpr = false);

        Result<const ast::Expression*> expression();

        Result<const ast::Expression*> primaryExpression();

        Result<const ast::Attribute*> attribute();

        Result<std::vector<const ast::Attribute*>> attributes();

        Result<const ast::IdentifierExpression*> identifierExpression();

        Result<const ast::LiteralExpression*> literal();

        Result<const ast::UnaryExpression*> unaryExpression();

        Result<const ast::ComponentDeclaration*> componentDeclaration();

        Result<const ast::ImportDeclaration*> importDeclaration();

        Result<const ast::Expression*> parseExpression1(
            const ast::Expression* lhs,
            size_t min_precedence,
            bool isExpr = false
        );

        void globalDeclaration();

        bool isQualifier(Token tok) const;

        void checkAttribute();

        template<class Fn, typename... Args>
        auto sync(Token::Type sync_token, Fn&& f, Args&&... args)
        {
            auto r = f(std::forward<Args>(args)...);

            if (!r.ok())
                while (!match(sync_token, false, false) && !match(Token::Type::kEOF, false, false))
                    advance();

            return r;
        }

        bool continueParsing();

        bool isOperator(const Token& tok, bool isWithinExpr);

        Associativity getAssociativity(const Token& tk);

        size_t getPrecedence(const Token& tk);

        const Token* peek(uint64_t off, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* match(std::string_view str, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* match(Token::Type token_type, bool skip_ws = true, bool skip_lnbrks = true);

        bool skipLinebreaks();

        bool skipWhitespaces();

        const Token* advance(size_t off = 1, bool skip_ws = true, bool skip_lnbrks = true);

        struct {
            size_t lineIndex;
            size_t columnIndex;
        } m_stats;
        
        int64_t m_tokenIdx;   

        ast::CloneContext m_ctx;

        std::unique_ptr<Lexer> m_lexer;
    };
}