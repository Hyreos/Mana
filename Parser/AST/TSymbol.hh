#pragma once

#include "TreeNode.hh"

#include <string>

namespace mona {
    class TSymbol : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kTSymbol };

        TSymbol(const std::string& identifier);

        DeletedUnique_T<TreeNode> clone();
    private:
        std::string m_identifier;
    };
}