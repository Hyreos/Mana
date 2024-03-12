#include "DivOp.hh"

namespace mana {
    DivOp::DivOp(std::unique_ptr<TreeNode> lhs, std::unique_ptr<TreeNode> rhs)
        : TreeNode(kind), m_lhs { std::move(lhs) }, m_rhs { std::move(rhs) }
    {
    }

    std::unique_ptr<TreeNode> DivOp::clone()
    {
        return std::make_unique<DivOp>(m_lhs->clone(), m_rhs->clone());
    }

    void DivOp::print(std::ostream& stream, size_t ident)
    {
        stream << "(";

        m_lhs->print(stream, ident);

        stream << " / ";

        m_rhs->print(stream, ident);

        stream << ")";
    }
}