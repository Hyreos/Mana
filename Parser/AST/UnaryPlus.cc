#include "UnaryPlus.hh"

#include "TreeTransverser.hh"

namespace mana::ast {
    UnaryPlus::UnaryPlus(std::unique_ptr<TreeNode> operand)
        : Unary { Unary::Kind::kPlus },
            m_operand{ std::move(operand) }
    {
    }

    std::unique_ptr<TreeNode> UnaryPlus::clone()
    {
        return std::make_unique<UnaryPlus>(m_operand->clone());
    }

    void UnaryPlus::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(";

        stream << "+";

        m_operand->print(stream, ident);

        stream << ")";
    }

    void UnaryPlus::accept(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }
}