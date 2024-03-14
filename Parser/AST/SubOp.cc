#include "SubOp.hh"

#include "TreeTransverser.hh"

namespace mana {
    SubOp::SubOp(std::unique_ptr<TreeNode> lhs, std::unique_ptr<TreeNode> rhs)
        : BinaryOp(kind, std::move(lhs), std::move(rhs))
    {
    }

    std::unique_ptr<TreeNode> SubOp::clone()
    {
        return std::make_unique<SubOp>(m_lhs->clone(), m_rhs->clone());
    }

    void SubOp::print(std::ostream& stream, size_t ident)
    {
        stream << "(";

        m_lhs->pprint(stream, ident);

        stream << " - ";

        m_rhs->pprint(stream, ident);

        stream << ")";
    }

    void SubOp::accept(TreeVisitor* visitor)
    {
        visitor->visitOperator(this);
    }
}