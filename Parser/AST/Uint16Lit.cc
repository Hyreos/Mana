#include "Uint16Lit.hh"

#include "TreeTransverser.hh"

namespace mana {
    Uint16Lit::Uint16Lit(uint16_t value) 
        : Literal(value),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Uint16Lit::clone()
    {
        return std::make_unique<Uint16Lit>(m_value);
    }

    void Uint16Lit::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(" << m_value << "_u16" << ")";
    }

    void Uint16Lit::accept(TreeVisitor* visitor)
    {
        visitor->visitLiteral(this);
    }
}