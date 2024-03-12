#pragma once

#include "TreeNode.hh"

#include <string>

namespace mana {
    class TSymbol : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kTSymbol };

        TSymbol(const std::string& identifier);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        std::string m_identifier;
    };
}