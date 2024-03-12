#include "SumOp.hh"

namespace mona {
    SumOp::SumOp(std::unique_ptr<TreeNode> lhs, std::unique_ptr<TreeNode> rhs)
        : TreeNode(kind),
            m_lhs{std::move(lhs)},
            m_rhs{std::move(rhs)}
    {
    }

    std::unique_ptr<TreeNode> SumOp::clone()
    {
        return std::make_unique<SumOp>(m_lhs->clone(), m_rhs->clone());
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