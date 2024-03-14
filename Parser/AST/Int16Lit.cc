#include "Int16Lit.hh"

#include "TreeTransverser.hh"

namespace mana {
    Int16Lit::Int16Lit(int16_t value) 
        : Literal(value),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Int16Lit::clone()
    {
        return std::make_unique<Int16Lit>(m_value);
    }

    void Int16Lit::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(" << m_value << "_i16" << ")";
    }

    void Int16Lit::accept(TreeVisitor* visitor)
    {
        visitor->visitLiteral(this);
    }
}