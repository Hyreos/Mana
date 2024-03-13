#include "CompFieldDecl.hh"

namespace mana {
    CompFieldDecl::CompFieldDecl(
        std::unique_ptr<TreeNode> type,
        const std::string& name,
        std::optional<std::string> propCppName
    ) 
        : TreeNode(kind),
            m_name{ name },
            m_type{ std::move(type) },
            m_cppPropName { propCppName }
    {
    }

    std::unique_ptr<TreeNode> CompFieldDecl::clone()
    {
        return std::make_unique<CompFieldDecl>(m_type->clone(), m_name);
    }

    void CompFieldDecl::print(std::ostream& stream, size_t ident)
    {
        for (auto& attr : attributes())
            attr->print(stream, ident);

        m_type->print(stream, ident);

        stream << " ";

        stream << m_name;

        if (m_cppPropName)
            stream << " -> " << *m_cppPropName;
    }

    const std::string& CompFieldDecl::name() const
    {
        return m_name;
    }
}