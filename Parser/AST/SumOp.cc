#include "SumOp.hh"

#include "TreeTransverser.hh"

namespace mana {
    SumOp::SumOp(std::unique_ptr<TreeNode> lhs, std::unique_ptr<TreeNode> rhs)
        : BinaryOp(kind, std::move(lhs), std::move(rhs))
    {
    }

    std::unique_ptr<TreeNode> SumOp::clone()
    {
        return std::make_unique<SumOp>(m_lhs->clone(), m_rhs->clone());
    }

    void SumOp::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(";

        m_lhs->print(stream, ident);

        stream << " + ";

        m_rhs->print(stream, ident);

        stream << ")";
    }

    void SumOp::accept(TreeVisitor* visitor)
    {
        visitor->visitOperator(this);
    }
}