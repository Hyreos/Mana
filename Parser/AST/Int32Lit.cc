#include "Int32Lit.hh"

namespace mana {
    Int32Lit::Int32Lit(int32_t value) 
        : TreeNode(kind),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Int32Lit::clone()
    {
        return std::make_unique<Int32Lit>(m_value);
    }

    void Int32Lit::print(std::ostream& stream, size_t ident)
    {
        stream << "(" << m_value << "_i32" << ")";
    }
}