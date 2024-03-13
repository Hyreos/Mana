#include "Int16Lit.hh"

namespace mana {
    Int16Lit::Int16Lit(int16_t value) 
        : TreeNode(kind),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Int16Lit::clone()
    {
        return std::make_unique<Int16Lit>(m_value);
    }

    void Int16Lit::print(std::ostream& stream, size_t ident)
    {
        stream << "(" << m_value << "_i16" << ")";
    }
}