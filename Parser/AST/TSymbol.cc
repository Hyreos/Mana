#include "TSymbol.hh"

namespace mona {
    TSymbol::TSymbol(const std::string& identifier) 
        : TreeNode(kind),
            m_identifier { identifier }
    {
    }

    DeletedUnique_T<TreeNode> TSymbol::clone()
    {
        return MakeUniquePtr<TSymbol>(m_identifier);
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