#pragma once

#include "AST/TreeNode.hh"
#include "AST/Attribute.hh"
#include "AST/CField.hh"
#include "AST/Component.hh"
#include "AST/Number.hh"
#include "AST/TSymbol.hh"

#include "Helpers/Deleted.hh"
#include "Helpers/Macros.hh"

#include "Lexer.hh"

namespace mona {
    class Parser {
    public:
        bool canPeek(size_t off, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* consumeCheck(Kind kind, size_t off = 1, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* consume(size_t off = 1, bool skip_ws = true, bool skip_lnbrks = true);

        Parser(const std::string& code);

        void parse();

        void registerErrorCallback(ErrorCallback_T callback);
    private:
        struct {
            size_t lineIndex;
            size_t columnIndex;
        } m_stats;

        int64_t m_tokenIdx;   

        mona::GrLexer m_lexer;

        DeletedUnique_T<TreeNode> processToken();

        const Token* peekExpected(size_t off, Kind kind, bool skip_ws = true, bool skip_lnbrks = true);

        const Token* peek(size_t off, bool skip_ws = true, bool skip_lnbrks = true);
    };
}