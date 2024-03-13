#pragma once

#include "AST/TreeNode.hh"
#include "AST/Attribute.hh"
#include "AST/CompFieldDecl.hh"
#include "AST/CompDecl.hh"

#include "AST/Int16Lit.hh"
#include "AST/Int32Lit.hh"
#include "AST/Int64Lit.hh"

#include "AST/Uint16Lit.hh"
#include "AST/Uint32Lit.hh"
#include "AST/Uint64Lit.hh"

#include "AST/TSymbol.hh"
#include "AST/ImportStat.hh"
#include "AST/SubOp.hh"
#include "AST/SumOp.hh"
#include "AST/MulOp.hh"
#include "AST/DivOp.hh"

#include "AST/UnaryMinus.hh"
#include "AST/UnaryPlus.hh"

#include "Helpers/Deleted.hh"
#include "Helpers/Macros.hh"

#include <queue>
#include <stack>

#include "Lexer.hh"

namespace mana {
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

        bool matches(int64_t off, Token::Type token_type, bool skip_ws = true, bool skip_lnbrks = true);

        bool canPeek(int64_t off, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* consumeCheck (Token::Type kind, size_t off = 1, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* consume(size_t off = 1, bool skip_ws = true, bool skip_lnbrks = true);

        bool isOperator(const Token& tok);

        Associativity getAssociativity(const Token& tk);

        size_t getPrecedence(const Token& tk);

        std::unique_ptr<TreeNode> parsePrimary();

        std::unique_ptr<TreeNode> parseExpression();

        std::unique_ptr<TreeNode> parseExpression1(
            std::unique_ptr<TreeNode> lhs,
            size_t min_precedence
        );

        const Token* peekExpected(int64_t off, Token::Type kind, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* peek(int64_t off, bool skip_ws = true, bool skip_lnbrks = true);

        struct {
            size_t lineIndex;
            size_t columnIndex;
        } m_stats;
        
        int64_t m_tokenIdx;   

        std::unique_ptr<mana::Lexer> m_lexer;
    };
}