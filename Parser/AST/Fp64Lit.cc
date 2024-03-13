#include "Fp64Lit.hh"

namespace mana {
    Fp64Lit::Fp64Lit(double value) 
        : TreeNode(kind),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Fp64Lit::clone()
    {
        return std::make_unique<Fp64Lit>(m_value);
    }

    void Fp64Lit::print(std::ostream& stream, size_t ident)
    {
        stream << "(" << m_value << "_fp64" << ")";
    }
}