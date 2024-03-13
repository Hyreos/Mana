#include "CompDecl.hh"

namespace mana {
    CompDecl::CompDecl(
        const std::string& name,
        std::vector<std::unique_ptr<TreeNode>> fields,
        std::vector<std::unique_ptr<TreeNode>> inheritances
    ) 
        : TreeNode(kind),
            m_fields{ std::move(fields) },
            m_inheritances { std::move(inheritances) },
            m_name{ name }
    {
    }

    std::unique_ptr<TreeNode> CompDecl::clone()
    {
        std::vector<std::unique_ptr<TreeNode>> cfields, cinheritances;

        for (auto& f : m_fields) cfields.push_back(f->clone());

        for (auto& h : m_inheritances) cinheritances.push_back(h->clone());

        return std::make_unique<CompDecl>(m_name, std::move(cfields), std::move(cinheritances));
    }

    void CompDecl::print(std::ostream& stream, size_t ident)
    {
        if (m_exported) stream << "export ";

        stream << "component " << m_name;

        if (!m_inheritances.empty()) stream << " : ";

        for (auto i = 0; i < m_inheritances.size(); i++) {
            if (i > 0) stream << ", ";
            m_inheritances[i]->pprint(stream, ident);
        }
        
        stream << " {" << std::endl;

        for (auto& f : m_fields) {
            std::cout << std::string(2 * ident + 2, ' ');
            
            f->pprint(stream, ident + 1);

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