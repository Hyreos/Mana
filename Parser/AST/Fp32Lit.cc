#include "Fp32Lit.hh"

#include "TreeTransverser.hh"

namespace mana {
    Fp32Lit::Fp32Lit(float value) 
        : Literal(value),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Fp32Lit::clone()
    {
        return std::make_unique<Fp32Lit>(m_value);
    }

    void Fp32Lit::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(" << m_value << "_f32" << ")";
    }

    void Fp32Lit::accept(TreeVisitor* visitor)
    {
        visitor->visitLiteral(this);
    }
}