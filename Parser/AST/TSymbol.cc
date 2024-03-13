#include "TSymbol.hh"

namespace mana {
    TSymbol::TSymbol(const std::string& identifier, bool isOptional) 
        : TreeNode(kind),
            m_identifier { identifier },
            m_isOptional{ isOptional }
    {
    }

    std::unique_ptr<TreeNode> TSymbol::clone()
    {
        return std::make_unique<TSymbol>(m_identifier, m_isOptional);
    }

    void TSymbol::print(std::ostream& stream, size_t ident)
    {
        for (auto& attr : attributes()) {
            attr->print(stream, ident);
            stream << " ";
        }

        stream << m_identifier;

        if (m_isOptional) stream << "?";
    }
}