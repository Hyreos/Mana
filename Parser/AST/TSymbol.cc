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
}