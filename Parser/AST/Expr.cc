#include "Expr.hh"

namespace mona {
    BinaryOp::BinaryOp(DeletedUnique_T<TreeNode> lhs, DeletedUnique_T<TreeNode> rhs) 
        : TreeNode(kind),
            m_lhs { std::move(lhs) },
            m_rhs { std::move(rhs) }
    {
    }

    DeletedUnique_T<TreeNode> BinaryOp::clone()
    {
        return MakeUniquePtr<BinaryOp>(m_lhs->clone(), m_rhs->clone());
    }

    void BinaryOp::print(std::ostream& stream) 
    {
        stream << "(";

        /*for (auto& e : m_exprs)
            e->print(stream);*/

        stream << ")";
    }
}