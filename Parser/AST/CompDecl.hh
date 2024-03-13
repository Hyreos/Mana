#pragma once

#include "TreeNode.hh"

namespace mana {
    class CompDecl : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kComponent };

        CompDecl(
            const std::string& name,
            std::vector<std::unique_ptr<TreeNode>> fields
        );

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        std::vector<std::unique_ptr<TreeNode>> m_fields;

        std::string m_name;
    };
}