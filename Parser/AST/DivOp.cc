#include "DivOp.hh"

namespace mona {
    DivOp::DivOp(DeletedUnique_T<TreeNode> lhs, DeletedUnique_T<TreeNode> rhs)
        : TreeNode(kind), m_lhs { std::move(lhs) }, m_rhs { std::move(rhs) }
    {
    }

    DeletedUnique_T<TreeNode> DivOp::clone()
    {
        return MakeUniquePtr<DivOp>(m_lhs->clone(), m_rhs->clone());
    }

    void DivOp::print(std::ostream& stream)
    {
        stream << "(";

        m_lhs->print(stream);

        stream << "/";

        m_rhs->print(stream);

        stream << ")";
    }
}