#include "Expr.hh"

#include "TreeTransverser.hh"

namespace mana {
    Expr::Expr(std::unique_ptr<TreeNode> operand)
        : TreeNode(kind),
            m_operand{std::move(operand)}
    {
    }

    std::unique_ptr<TreeNode> Expr::clone()
    {
        return std::make_unique<Expr>(m_operand->clone());
    }

    void Expr::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(";

        m_operand->print(stream, ident);

        stream << ")";
    }

    void Expr::accept(TreeVisitor* visitor)
    {
        visitor->visitExpression(this);
    }
}