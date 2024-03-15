#include "Fp64Lit.hh"

#include "TreeTransverser.hh"

namespace mana::ast {
    Fp64Lit::Fp64Lit(double value) 
        : Literal(value),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Fp64Lit::clone()
    {
        return std::make_unique<Fp64Lit>(m_value);
    }

    void Fp64Lit::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(" << m_value << "_fp64" << ")";
    }

    void Fp64Lit::accept(TreeVisitor* visitor)
    {
        visitor->visit(static_cast<Literal*>(this));
    }
}