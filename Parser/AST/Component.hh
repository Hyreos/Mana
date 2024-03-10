#pragma once

#include "TreeNode.hh"

namespace mona {
    class Component : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kComponent };

        Component(
            std::vector<DeletedUnique_T<TreeNode>> fields
        );

        DeletedUnique_T<TreeNode> clone() override;
    private:
        std::vector<DeletedUnique_T<TreeNode>> m_fields;
    };
}