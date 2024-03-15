#include "TSymbol.hh"

#include "TreeTransverser.hh"

namespace mana::ast {
    TSymbol::TSymbol(const std::string& identifier) 
        : TreeNode(baseType),
            m_identifier { identifier }
    {
    }

    std::unique_ptr<TreeNode> TSymbol::clone()
    {
        return std::make_unique<TSymbol>(m_identifier);
    }

    void TSymbol::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << m_identifier;
    }

    void TSymbol::accept(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }
}