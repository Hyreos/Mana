#include "DivOp.hh"

#include "TreeTransverser.hh"

namespace mana {
    DivOp::DivOp(std::unique_ptr<TreeNode> lhs, std::unique_ptr<TreeNode> rhs)
        : BinaryOp(kind, std::move(lhs), std::move(rhs))
    {
    }

    std::unique_ptr<TreeNode> DivOp::clone()
    {
        return std::make_unique<DivOp>(m_lhs->clone(), m_rhs->clone());
    }

    void DivOp::print(std::ostream& stream, size_t ident)
    {
        stream << "(";

        m_lhs->pprint(stream, ident);

        stream << " / ";

        m_rhs->pprint(stream, ident);

        stream << ")";
    }

    void DivOp::accept(TreeVisitor* visitor)
    {
        visitor->visitOperator(this);
    }
}