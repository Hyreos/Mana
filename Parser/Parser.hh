#pragma once

#include "AST/TreeNode.hh"
#include "AST/Attribute.hh"
#include "AST/CField.hh"
#include "AST/Component.hh"
#include "AST/Number.hh"
#include "AST/TSymbol.hh"
#include "AST/SubOp.hh"
#include "AST/SumOp.hh"
#include "AST/MulOp.hh"
#include "AST/DivOp.hh"

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

        bool canPeek(int64_t off, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* consumeCheck(Kind kind, size_t off = 1, bool skip_ws = true, bool skip_lnbrks = true);

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

        const Token* peekExpected(int64_t off, Kind kind, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* peek(int64_t off, bool skip_ws = true, bool skip_lnbrks = true);

        struct {
            size_t lineIndex;
            size_t columnIndex;
        } m_stats;
        
        int64_t m_tokenIdx;   

        std::unique_ptr<mana::GrLexer> m_lexer;
    };
}