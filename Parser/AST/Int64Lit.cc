#include "Int64Lit.hh"

namespace mana {
    Int64Lit::Int64Lit(int64_t value) 
        : TreeNode(kind),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Int64Lit::clone()
    {
        return std::make_unique<Int64Lit>(m_value);
    }

    void Int64Lit::print(std::ostream& stream, size_t ident)
    {
        stream << "(" << m_value << "_i64" << ")";
    }
}