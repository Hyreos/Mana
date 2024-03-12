#pragma once

#include "TreeNode.hh"

#include <string>

namespace mona {
    class SumOp : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kAdd };

        SumOp(DeletedUnique_T<TreeNode> lhs, DeletedUnique_T<TreeNode> rhs);

        DeletedUnique_T<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        DeletedUnique_T<TreeNode> m_lhs;
        DeletedUnique_T<TreeNode> m_rhs;
    };
}