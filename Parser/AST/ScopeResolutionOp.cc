#include "ScopeResolutionOp.hh"

#include "TreeTransverser.hh"

namespace mana {
    ScopeResolutionOp::ScopeResolutionOp(std::unique_ptr<TreeNode> lhs, std::unique_ptr<TreeNode> rhs)
        : TreeNode(kind),
            m_lhs{std::move(lhs)},
            m_rhs{std::move(rhs)}
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
        visitor->visitOperator(this);
    }
}