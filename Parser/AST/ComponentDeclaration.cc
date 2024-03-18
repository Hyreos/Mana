#include "ComponentDeclaration.hh"

namespace mana::ast {
    ComponentDeclaration::ComponentDeclaration(
        const std::string& name,
        std::vector<const Declaration*> fields,
        std::vector<const IdentifierExpression*> inheritances,
        bool isExported,
        std::vector<const Attribute*> attributes
    ) 
        : m_fields{ std::move(fields) },
            m_inheritances { std::move(inheritances) },
            m_exported { isExported },
            m_attributes { attributes }
    {
        m_name = name;
    }

    const ComponentDeclaration* ComponentDeclaration::clone(CloneContext& ctx) const
    {
        return ctx.create<ComponentDeclaration>(
            m_name, 
            ctx.clone(m_fields), 
            ctx.clone(m_inheritances), 
            m_exported,
            ctx.clone(m_attributes)
        );
    }

    void ComponentDeclaration::print(std::ostream& stream, size_t ident) const
    {
        for (auto& attr : m_attributes) {
            attr->print(stream, ident);
            
            stream << " ";
        }

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