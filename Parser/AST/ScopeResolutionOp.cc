#include "ScopeResolutionOp.hh"

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
        stream << "(";

        m_lhs->pprint(stream, ident);

        stream << "::";

        m_rhs->pprint(stream, ident);

        stream << ")";
    }
}