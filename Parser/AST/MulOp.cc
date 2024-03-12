#include "MulOp.hh"

namespace mona {
    MulOp::MulOp(DeletedUnique_T<TreeNode> lhs, DeletedUnique_T<TreeNode> rhs)
        : TreeNode(kind),
            m_lhs{std::move(lhs)},
            m_rhs{std::move(rhs)}
    {
    }

    DeletedUnique_T<TreeNode> MulOp::clone()
    {
        return MakeUniquePtr<MulOp>(m_lhs->clone(), m_rhs->clone());
    }

    void MulOp::print(std::ostream& stream, size_t ident)
    {
        stream << "(";

        m_lhs->print(stream, ident);

        stream << "*";

        m_rhs->print(stream, ident);
        
        stream << ")";
    }
}