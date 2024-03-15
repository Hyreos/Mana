#include "StrLit.hh"

#include "TreeTransverser.hh"

namespace mana::ast {
    StrLit::StrLit(const std::string& str) 
        : Literal(str)
    {
    }

    std::unique_ptr<TreeNode> StrLit::clone()
    {
        return std::make_unique<StrLit>(m_value);
    }

    void StrLit::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "(" << m_value << "_str" << ")";
    }

    void StrLit::accept(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }
}