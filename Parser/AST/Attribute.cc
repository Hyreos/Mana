#include "Attribute.hh"

namespace mona {
    Attribute::Attribute(
        const std::string& name,
        DeletedUnique_T<TreeNode> value
    ) 
        : TreeNode(kind),
            m_name { name },
            m_value { std::move(value) } 
    {
    }

    DeletedUnique_T<TreeNode> Attribute::clone()
    {
        return MakeUniquePtr<Attribute>(m_name, m_value->clone());
    }
}