#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

namespace mana {
    class Int32Lit : public TreeNode {
    public:
        static constexpr TreeNode::Type kind { TreeNode::Type::kInt32Lit };

        Int32Lit(int32_t value);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        int32_t m_value;
    };
}