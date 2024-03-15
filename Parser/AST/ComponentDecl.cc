#include "ComponentDecl.hh"

#include "TreeTransverser.hh"

namespace mana::ast {
    ComponentDecl::ComponentDecl(
        const std::string& name,
        std::vector<std::unique_ptr<TreeNode>> fields,
        std::vector<std::unique_ptr<TreeNode>> inheritances,
        bool isExported
    ) 
        : Declaration(Declaration::Kind::kComponent),
            m_fields{ std::move(fields) },
            m_inheritances { std::move(inheritances) },
            m_name{ name },
            m_exported { isExported }
    {
    }

    std::unique_ptr<TreeNode> ComponentDecl::clone()
    {
        std::vector<std::unique_ptr<TreeNode>> cfields, cinheritances;

        for (auto& f : m_fields) cfields.push_back(f->clone());

        for (auto& h : m_inheritances) cinheritances.push_back(h->clone());

        return std::make_unique<ComponentDecl>(
            m_name, 
            std::move(cfields), 
            std::move(cinheritances), 
            m_exported
        );
    }

    void ComponentDecl::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        if (m_exported) stream << "export ";

        stream << "component " << m_name;

        if (!m_inheritances.empty()) stream << " : ";

        for (auto i = 0; i < m_inheritances.size(); i++) {
            if (i > 0) stream << ", ";
            m_inheritances[i]->print(stream, ident);
        }
        
        stream << " {" << std::endl;

        for (auto& f : m_fields) {
            std::cout << std::string(2 * ident + 2, ' ');
            
            f->print(stream, ident + 1);

            stream << std::endl;
        }
        
        stream << "}";
    }

    void ComponentDecl::setExportStatus(bool value)
    {
        m_exported = value;
    }

    bool ComponentDecl::isExported() const
    {
        return m_exported;
    }

    void ComponentDecl::accept(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }
}