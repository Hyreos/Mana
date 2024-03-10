#include "CField.hh"

namespace mona {
    CField::CField(
        DeletedUnique_T<TreeNode> type,
        const std::string& name
    ) 
        : TreeNode(kind),
            m_name{ name },
            m_type{ std::move(type) }
    {
    }

    DeletedUnique_T<TreeNode> CField::clone()
    {
        return MakeUniquePtr<CField>(m_type->clone(), m_name);
    }
}