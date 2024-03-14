#include "Int32Lit.hh"

#include "TreeTransverser.hh"

namespace mana {
    Int32Lit::Int32Lit(int32_t value) 
        : Literal(value),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Int32Lit::clone()
    {
        return std::make_unique<Int32Lit>(m_value);
    }

    void Int32Lit::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(" << m_value << "_i32" << ")";
    }

    void Int32Lit::accept(TreeVisitor* visitor)
    {
        visitor->visitLiteral(this);
    }
}