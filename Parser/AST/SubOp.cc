#include "SubOp.hh"

namespace mana {
    SubOp::SubOp(std::unique_ptr<TreeNode> lhs, std::unique_ptr<TreeNode> rhs)
        : TreeNode(kind),
            m_lhs{std::move(lhs)},
            m_rhs{std::move(rhs)}
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
}