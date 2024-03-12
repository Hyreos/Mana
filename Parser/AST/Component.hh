#pragma once

#include "TreeNode.hh"

namespace mona {
    class Component : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kComponent };

        Component(
            const std::string& name,
            std::vector<DeletedUnique_T<TreeNode>> fields
        );

        DeletedUnique_T<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        std::vector<DeletedUnique_T<TreeNode>> m_fields;

        std::string m_name;
    };
}