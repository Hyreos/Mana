#pragma once

#include "TreeNode.hh"

#include <string>

namespace mona {
    class BinaryOp : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kExpr };

        BinaryOp(DeletedUnique_T<TreeNode> lhs, DeletedUnique_T<TreeNode> rhs);

        DeletedUnique_T<TreeNode> clone() override;

        void print(std::ostream& stream) override;
    private:
        DeletedUnique_T<TreeNode> m_lhs, m_rhs;
    };
}