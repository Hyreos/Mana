#include "CompDecl.hh"

namespace mana {
    CompDecl::CompDecl(
        const std::string& name,
        std::vector<std::unique_ptr<TreeNode>> fields
    ) 
        : TreeNode(kind),
            m_fields{ std::move(fields) },
            m_name{ name }
    {
    }

    std::unique_ptr<TreeNode> CompDecl::clone()
    {
        std::vector<std::unique_ptr<TreeNode>> cfields;

        for (auto& f : m_fields) cfields.push_back(f->clone());

        return std::make_unique<CompDecl>(m_name, std::move(cfields));
    }

    void CompDecl::print(std::ostream& stream, size_t ident)
    {
        if (m_exported) stream << "export ";

        stream << "component " << m_name << " {" << std::endl;

        for (auto& f : m_fields) {
            std::cout << std::string(2 * ident + 2, ' ');
            
            f->print(stream, ident + 1);

            stream << std::endl;
        }
        
        stream << "}";
    }

    void CompDecl::setExportStatus(bool value)
    {
        m_exported = value;
    }

    bool CompDecl::isExported() const
    {
        return m_exported;
    }
}