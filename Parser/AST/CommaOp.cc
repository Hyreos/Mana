#include "CommaOp.hh"

#include "TreeTransverser.hh"

namespace mana {
    CommaOp::CommaOp(std::unique_ptr<TreeNode> lhs, std::unique_ptr<TreeNode> rhs)
        : BinaryOp(kind, std::move(lhs), std::move(rhs))
    {
    }

    std::unique_ptr<TreeNode> CommaOp::clone()
    {
        return std::make_unique<CommaOp>(m_lhs->clone(), m_rhs->clone());
    }

    void CommaOp::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(";

        m_lhs->print(stream, ident);

        stream << ", ";

        m_rhs->print(stream, ident);

        stream << ")";
    }

    void CommaOp::accept(TreeVisitor* visitor)
    {
        visitor->visitOperator(this);
    }
}