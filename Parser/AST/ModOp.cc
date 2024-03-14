#include "ModOp.hh"

#include "TreeTransverser.hh"

namespace mana {
    ModOp::ModOp(std::unique_ptr<TreeNode> lhs, std::unique_ptr<TreeNode> rhs)
        : BinaryOp(kind, std::move(lhs), std::move(rhs))
    {
    }

    std::unique_ptr<TreeNode> ModOp::clone()
    {
        return std::make_unique<ModOp>(m_lhs->clone(), m_rhs->clone());
    }

    void ModOp::print(std::ostream& stream, size_t ident)
    {
        stream << "(";

        m_lhs->pprint(stream, ident);

        stream << " % ";

        m_rhs->pprint(stream, ident);

        stream << ")";
    }

    void ModOp::accept(TreeVisitor* visitor)
    {
        visitor->visitOperator(this);
    }
}