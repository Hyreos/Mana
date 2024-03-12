#include "Number.hh"

namespace mona {
    Number::Number(const std::string& identifier) 
        : TreeNode(kind),
            m_identifier { identifier }
    {
    }

    std::unique_ptr<TreeNode> Number::clone()
    {
        return std::make_unique<Number>(m_identifier);
    }

    void Number::print(std::ostream& stream, size_t ident)
    {
        stream << m_identifier;
    }
}