#include "TSymbol.hh"

namespace mona {
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
        for (auto& attr : attributes()) {
            attr->print(stream, ident);
            stream << " ";
        }

        stream << m_identifier;
    }
}