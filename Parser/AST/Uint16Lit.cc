#include "Uint16Lit.hh"

namespace mana {
    Uint16Lit::Uint16Lit(uint16_t value) 
        : TreeNode(kind),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Uint16Lit::clone()
    {
        return std::make_unique<Uint16Lit>(m_value);
    }

    void Uint16Lit::print(std::ostream& stream, size_t ident)
    {
        stream << "(" << m_value << "_u16" << ")";
    }
}