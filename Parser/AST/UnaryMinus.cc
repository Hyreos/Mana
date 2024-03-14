#include "UnaryMinus.hh"

#include "TreeTransverser.hh"

namespace mana {
    UnaryMinus::UnaryMinus(std::unique_ptr<TreeNode> operand)
        : TreeNode(kind),
            m_operand{std::move(operand)}
    {
    }

    std::unique_ptr<TreeNode> UnaryMinus::clone()
    {
        return std::make_unique<UnaryMinus>(m_operand->clone());
    }

    void UnaryMinus::print(std::ostream& stream, size_t ident)
    {
        stream << "(";

        stream << "-";

        m_operand->pprint(stream, ident);

        stream << ")";
    }

    void UnaryMinus::accept(TreeVisitor* visitor)
    {
        visitor->visitOperator(this);
    }
}