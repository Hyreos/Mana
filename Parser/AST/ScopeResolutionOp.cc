#include "ScopeResolutionOp.hh"

#include "TreeTransverser.hh"

namespace mana::ast {
    ScopeResolutionOp::ScopeResolutionOp(std::unique_ptr<TreeNode> lhs, std::unique_ptr<TreeNode> rhs)
        : BinaryOp { BinaryOp::TypeMask::kScopeRes, std::move(lhs), std::move(rhs) }
    {
    }

    std::unique_ptr<TreeNode> ScopeResolutionOp::clone()
    {
        return std::make_unique<ScopeResolutionOp>(m_lhs->clone(), m_rhs->clone());
    }

    void ScopeResolutionOp::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(";

        m_lhs->print(stream, ident);

        stream << "::";

        m_rhs->print(stream, ident);

        stream << ")";
    }

    void ScopeResolutionOp::accept(TreeVisitor* visitor)
    {
        visitor->visit(static_cast<BinaryOp*>(this));
    }
}