#include "Fp32Lit.hh"

namespace mana {
    Fp32Lit::Fp32Lit(float value) 
        : TreeNode(kind),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Fp32Lit::clone()
    {
        return std::make_unique<Fp32Lit>(m_value);
    }

    void Fp32Lit::print(std::ostream& stream, size_t ident)
    {
        stream << "(" << m_value << "_f32" << ")";
    }
}