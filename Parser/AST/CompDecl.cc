#include "CompDecl.hh"

namespace mana {
    CompDecl::CompDecl(
        const std::string& name,
        std::vector<std::unique_ptr<TreeNode>> fields,
        const std::vector<std::string>& inheritances,
        const std::vector<std::string>& cpp_inheritances
    ) 
        : TreeNode(kind),
            m_fields{ std::move(fields) },
            m_name{ name },
            m_inheritances { inheritances },
            m_cppInheritances { cpp_inheritances }
    {
    }

    std::unique_ptr<TreeNode> CompDecl::clone()
    {
        std::vector<std::unique_ptr<TreeNode>> cfields;

        for (auto& f : m_fields) cfields.push_back(f->clone());

        return std::make_unique<CompDecl>(m_name, std::move(cfields), m_inheritances, m_cppInheritances);
    }

    void CompDecl::print(std::ostream& stream, size_t ident)
    {
        if (m_exported) stream << "export ";

        stream << "component " << m_name;

        if (!m_inheritances.empty() || !m_cppInheritances.empty()) stream << " : ";

        for (auto i = 0; i < m_inheritances.size(); i++) {
            if (i > 0) stream << ", ";
            
            stream << m_inheritances[i];
        }

        for (auto i = 0; i < m_cppInheritances.size(); i++) {
            if (i > 0 || !m_inheritances.empty()) 
                stream << ", ";

            stream << "cpp ";
            stream << m_cppInheritances[i];
        }
        
        stream << " {" << std::endl;

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