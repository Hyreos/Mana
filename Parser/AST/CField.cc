#include "CField.hh"

namespace mana {
    CField::CField(
        std::unique_ptr<TreeNode> type,
        const std::string& name
    ) 
        : TreeNode(kind),
            m_name{ name },
            m_type{ std::move(type) }
    {
    }

    std::unique_ptr<TreeNode> CField::clone()
    {
        return std::make_unique<CField>(m_type->clone(), m_name);
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