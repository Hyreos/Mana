#include "SumOp.hh"

namespace mona {
    SumOp::SumOp(DeletedUnique_T<TreeNode> lhs, DeletedUnique_T<TreeNode> rhs)
        : TreeNode(kind),
            m_lhs{std::move(lhs)},
            m_rhs{std::move(rhs)}
    {
    }

    DeletedUnique_T<TreeNode> SumOp::clone()
    {
        return MakeUniquePtr<SumOp>(m_lhs->clone(), m_rhs->clone());
    }

    void SumOp::print(std::ostream& stream, size_t ident)
    {
        stream << "(";

        m_lhs->print(stream, ident);

        stream << "+";

        m_rhs->print(stream, ident);

        stream << ")";
    }
}