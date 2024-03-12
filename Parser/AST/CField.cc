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

    void CField::print(std::ostream& stream, size_t ident)
    {
        for (auto& attr : attributes())
            attr->print(stream, ident);

        m_type->print(stream, ident);

        stream << " ";

        stream << m_name;
    }
}