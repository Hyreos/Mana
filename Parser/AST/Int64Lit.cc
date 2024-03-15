#include "Int64Lit.hh"

#include "TreeTransverser.hh"

namespace mana::ast {
    Int64Lit::Int64Lit(int64_t value) 
        : Literal(value),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Int64Lit::clone()
    {
        return std::make_unique<Int64Lit>(m_value);
    }

    void Int64Lit::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(" << m_value << "_i64" << ")";
    }

    void Int64Lit::accept(TreeVisitor* visitor)
    {
        visitor->visit(static_cast<Literal*>(this));
    }
}