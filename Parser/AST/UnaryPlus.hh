#pragma once

#include "TreeNode.hh"

#include <string>

namespace mana {
    class UnaryPlus : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kUnaryPlus };

        UnaryPlus(std::unique_ptr<TreeNode> operand);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        std::unique_ptr<TreeNode> m_operand;;
    };
}