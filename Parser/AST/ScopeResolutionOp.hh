#pragma once

#include "TreeNode.hh"

#include <string>

namespace mana {
    class ScopeResolutionOp : public TreeNode {
    public:
        static constexpr TreeNode::Type kind { TreeNode::Type::kScopeResolution};

        ScopeResolutionOp(std::unique_ptr<TreeNode> lhs, std::unique_ptr<TreeNode> rhs);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void accept(TreeVisitor* visitor) override;
    private:
        std::unique_ptr<TreeNode> m_lhs;
        std::unique_ptr<TreeNode> m_rhs;
    };
}