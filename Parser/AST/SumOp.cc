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
        stream << "(";

        m_lhs->pprint(stream, ident);

        stream << " + ";

        m_rhs->pprint(stream, ident);

        stream << ")";
    }

    void SumOp::accept(TreeVisitor* visitor)
    {
        visitor->visitOperator(this);
    }
}