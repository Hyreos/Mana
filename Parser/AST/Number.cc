#include "Number.hh"

namespace mona {
    Number::Number(const std::string& identifier) 
        : TreeNode(kind),
            m_identifier { identifier }
    {
    }

    DeletedUnique_T<TreeNode> Number::clone()
    {
        return MakeUniquePtr<Number>(m_identifier);
    }

    void Number::print(std::ostream& stream, size_t ident)
    {
        stream << m_identifier;
    }
}