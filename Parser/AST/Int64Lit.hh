#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

namespace mana {
    class Int64Lit : public TreeNode {
    public:
        static constexpr TreeNode::Type kind { TreeNode::Type::kInt64Lit };

        Int64Lit(int64_t value);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        int64_t m_value;
    };
}