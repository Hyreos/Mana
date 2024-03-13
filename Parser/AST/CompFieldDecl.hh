#pragma once

#include "TreeNode.hh"

#include <string>

namespace mana {
    class CompFieldDecl : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kComponentField };

        CompFieldDecl(
            std::unique_ptr<TreeNode> type,
            const std::string& name
        );

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        std::string m_name;
        std::unique_ptr<TreeNode> m_type;
    };
}