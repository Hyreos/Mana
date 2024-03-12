#pragma once

#include "TreeNode.hh"

#include <string>

namespace mona {
    class CField : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kComponentField };

        CField(
            DeletedUnique_T<TreeNode> type,
            const std::string& name
        );

        DeletedUnique_T<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        std::string m_name;
        DeletedUnique_T<TreeNode> m_type;
    };
}