#include "ComponentDeclaration.hh"

namespace mana::ast {
    ComponentDeclaration::ComponentDeclaration(
        const std::string& name,
        std::vector<const Declaration*> fields,
        std::vector<const IdentifierExpression*> inheritances,
        bool isExported
    ) 
        : m_fields{ std::move(fields) },
            m_inheritances { std::move(inheritances) },
            m_exported { isExported }
    {
        m_name = name;
    }

    const ComponentDeclaration* ComponentDeclaration::clone(CloneContext& ctx) const
    {
        std::vector<const Declaration*> cfields;
        std::vector<const IdentifierExpression*> cinheritances;

        for (auto& f : m_fields) cfields.push_back(ctx.clone(f));
        for (auto& h : m_inheritances) cinheritances.push_back(ctx.clone(h));

        return ctx.create<ComponentDeclaration>(
            m_name, 
            std::move(cfields), 
            std::move(cinheritances), 
            m_exported
        );
    }

    void ComponentDeclaration::print(std::ostream& stream, size_t ident) const
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

    void ComponentDeclaration::setExportStatus(bool value)
    {
        m_exported = value;
    }

    bool ComponentDeclaration::isExported() const
    {
        return m_exported;
    }
}