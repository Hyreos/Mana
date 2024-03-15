#pragma once

#include "AST/TreeNode.hh"
#include "AST/Attribute.hh"

#include "AST/ImportDecl.hh"
#include "AST/ComponentDecl.hh"
#include "AST/MemberDecl.hh"

#include "AST/Int16Lit.hh"
#include "AST/Int32Lit.hh"
#include "AST/Int64Lit.hh"

#include "AST/Uint16Lit.hh"
#include "AST/Uint32Lit.hh"
#include "AST/Uint64Lit.hh"

#include "AST/Fp32Lit.hh"
#include "AST/Fp64Lit.hh"

#include "AST/StrLit.hh"

#include "AST/TSymbol.hh"
#include "AST/SubOp.hh"
#include "AST/SumOp.hh"
#include "AST/MulOp.hh"
#include "AST/DivOp.hh"
#include "AST/ScopeResolutionOp.hh"
#include "AST/CommaOp.hh"
#include "AST/Qual.hh"

#include "AST/Type.hh"

#include "AST/Expr.hh"

#include "AST/UnaryMinus.hh"
#include "AST/UnaryPlus.hh"

#include "Helpers/Deleted.hh"
#include "Helpers/Macros.hh"

#include <queue>
#include <stack>

#include "Lexer.hh"

namespace mana {
    template<typename T>
    struct Result {
        Result() : 
            errored{ false }, 
            matched { false }
        {
        }

        Result(bool _erroed, bool _matched) : 
            errored { _erroed }, 
            matched { _matched }
        {
        }

        Result(bool _erroed, bool _matched, T&& value) : 
            errored { _erroed }, 
            matched { _matched }, 
            value { std::move(value) }
        {
        }

        T& unwrap() 
        {
            return *value;
        }

        T& operator*()
        {
            return *value;
        }

        T* operator->()
        {
            return &*value;
        }

        bool errored;
        bool matched;
        std::optional<T> value;
    };

    template<typename T>
    using ASTResult_T = Result<std::unique_ptr<T>>;

    enum class ParseStatus {
        kSuccess,
        kFailure,
        kError,
        kContinue
    };

    template<typename T>
    ASTResult_T<T> ResultOk(std::unique_ptr<T>&& value)
    {
        return ASTResult_T<T>(false, true, std::move(value));
    }

    template<typename T>
    ASTResult_T<T> ResultFail()
    {
        return ASTResult_T<T>(true, false);
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

        std::unique_ptr<ast::TreeNode> parsePrimary();

        std::unique_ptr<ast::TreeNode> parseExpression(bool isExpr = false);

        ASTResult_T<ast::Attribute> parseAttribute();

        std::vector<std::unique_ptr<ast::Attribute>> attributes();

        ASTResult_T<ast::TreeNode> typeOrSymbol();

        ASTResult_T<ast::Literal> checkLiteral();

        ASTResult_T<ast::Expr> checkExpr();

        ASTResult_T<ast::UnaryMinus> checkUnary();

        bool isQualifier(Token tok) const;

        ASTResult_T<ast::ComponentDecl> componentDecl();

        ASTResult_T<ast::ImportDecl> importDecl();

        void globalDecl();

        void checkAttribute();

        template<class Callable, typename... Args>
        void sync(Token::Type sync_token, Callable&& f, Args&&... args)
        {
            auto r = f(std::forward<Args>(args)...);

            if (r == ParseStatus::kFailure)
                while (!matches(1, sync_token, false, false))                   
                    advance();
        }

        std::unique_ptr<ast::TreeNode> parseExpression1(
            std::unique_ptr<ast::TreeNode> lhs,
            size_t min_precedence,
            bool isExpr = false
        );

        bool continueParsing();

        bool isOperator(const Token& tok, bool isWithinExpr);

        Associativity getAssociativity(const Token& tk);

        size_t getPrecedence(const Token& tk);

        const Token* peek(uint64_t off, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* matches(int64_t off, std::string_view str, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* matches(int64_t off, Token::Type token_type, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* advance(size_t off = 1, bool skip_ws = true, bool skip_lnbrks = true);

        struct {
            size_t lineIndex;
            size_t columnIndex;
        } m_stats;
        
        int64_t m_tokenIdx;   

        std::unique_ptr<Lexer> m_lexer;
    };
}