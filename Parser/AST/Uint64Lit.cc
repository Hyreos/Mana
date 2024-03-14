#include "Uint64Lit.hh"

#include "TreeTransverser.hh"

namespace mana {
    Uint64Lit::Uint64Lit(uint64_t value) 
        : Literal(value),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Uint64Lit::clone()
    {
        return std::make_unique<Uint64Lit>(m_value);
    }

    void Uint64Lit::print(std::ostream& stream, size_t ident)
    {
        stream << "(" << m_value << "_u64" << ")";
    }

    void Uint64Lit::accept(TreeVisitor* visitor)
    {
        visitor->visitLiteral(this);
    }
}