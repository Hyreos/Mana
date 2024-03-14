#include "MulOp.hh"

#include "TreeTransverser.hh"

namespace mana {
    MulOp::MulOp(std::unique_ptr<TreeNode> lhs, std::unique_ptr<TreeNode> rhs)
        : BinaryOp(kind, std::move(lhs), std::move(rhs))
    {
    }

    std::unique_ptr<TreeNode> MulOp::clone()
    {
        return std::make_unique<MulOp>(m_lhs->clone(), m_rhs->clone());
    }

    void MulOp::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(";

        m_lhs->print(stream, ident);

        stream << " * ";

        m_rhs->print(stream, ident);
        
        stream << ")";
    }

    void MulOp::accept(TreeVisitor* visitor)
    {
        visitor->visitOperator(this);
    }
}