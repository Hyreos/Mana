#include "Uint32Lit.hh"

#include "TreeTransverser.hh"

namespace mana {
    Uint32Lit::Uint32Lit(uint32_t value) 
        : Literal(value),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Uint32Lit::clone()
    {
        return std::make_unique<Uint32Lit>(m_value);
    }

    void Uint32Lit::print(std::ostream& stream, size_t ident)
    {
        stream << "(" << m_value << "_u32" << ")";
    }

    void Uint32Lit::accept(TreeVisitor* visitor)
    {
        visitor->visitLiteral(this);
    }
}