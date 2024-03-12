#include "SubOp.hh"

namespace mona {
    SubOp::SubOp(DeletedUnique_T<TreeNode> lhs, DeletedUnique_T<TreeNode> rhs)
        : TreeNode(kind),
            m_lhs{std::move(lhs)},
            m_rhs{std::move(rhs)}
    {
    }

    DeletedUnique_T<TreeNode> SubOp::clone()
    {
        return MakeUniquePtr<SubOp>(m_lhs->clone(), m_rhs->clone());
    }

    void SubOp::print(std::ostream& stream)
    {
        stream << "(";

        m_lhs->print(stream);

        stream << "-";

        m_rhs->print(stream);

        stream << ")";
    }
}