#include "Uint64Lit.hh"

namespace mana {
    Uint64Lit::Uint64Lit(uint64_t value) 
        : TreeNode(kind),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Uint64Lit::clone()
    {
        return std::make_unique<Uint64Lit>(m_value);
    }

    void Uint64Lit::print(std::ostream& stream, size_t ident)
    {
        stream << "(" << m_value << ")";
    }
}