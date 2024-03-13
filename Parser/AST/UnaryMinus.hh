#pragma once

#include "TreeNode.hh"

#include <string>

namespace mana {
    class UnaryMinus : public TreeNode {
    public:
        static constexpr TreeNode::Type kind { TreeNode::Type::kUnaryMinus };

        UnaryMinus(std::unique_ptr<TreeNode> operand);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        std::unique_ptr<TreeNode> m_operand;;
    };
}