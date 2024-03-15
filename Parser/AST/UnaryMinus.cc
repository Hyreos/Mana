#include "UnaryMinus.hh"

#include "TreeTransverser.hh"

namespace mana::ast {
    UnaryMinus::UnaryMinus(std::unique_ptr<TreeNode> operand)
        : Unary { Unary::Kind::kMinus },
            m_operand{ std::move(operand) }
    {
    }

    std::unique_ptr<TreeNode> UnaryMinus::clone()
    {
        return std::make_unique<UnaryMinus>(m_operand->clone());
    }

    void UnaryMinus::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(";

        stream << "-";

        m_operand->print(stream, ident);

        stream << ")";
    }

    void UnaryMinus::accept(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }
}