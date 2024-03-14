#include "TSymbol.hh"

#include "TreeTransverser.hh"

namespace mana {
    TSymbol::TSymbol(const std::string& identifier) 
        : TreeNode(kind),
            m_identifier { identifier }
    {
    }

    std::unique_ptr<TreeNode> TSymbol::clone()
    {
        return std::make_unique<TSymbol>(m_identifier);
    }

    void TSymbol::print(std::ostream& stream, size_t ident)
    {
        stream << m_identifier;
    }

    void TSymbol::accept(TreeVisitor* visitor)
    {
        visitor->visitSymbol(this);
    }
}