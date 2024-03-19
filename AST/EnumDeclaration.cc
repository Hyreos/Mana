#include "EnumDeclaration.hh"

namespace mana::ast {
    EnumDeclaration::EnumDeclaration(
        const std::string& name,
        const std::vector<const EEntryDeclaration*>& entries,
        const std::vector<const Attribute*>& attributes
    ) 
        : m_entries{ entries },
            m_attributes { attributes },
            m_name { name }
    {
    }

    const EnumDeclaration* EnumDeclaration::clone(CloneContext& ctx) const
    {
        return ctx.create<EnumDeclaration>(
            m_name, 
            ctx.clone(m_entries), 
            ctx.clone(m_attributes)
        );
    }

    void EnumDeclaration::print(std::ostream& stream, size_t ident) const
    {
        for (auto& attr : m_attributes) {
            attr->print(stream, ident);
            
            stream << " ";
        }

        stream << "enum " << m_name << " {" << std::endl;

        size_t i = 0;

        for (auto& entry : m_entries) {
            if (i++ > 0) stream << ", ";

            std::cout << std::string(2 * ident + 2, ' ');
            
            entry->print(stream, ident + 1);
        }

        stream << std::endl;
        
        stream << "}" << std::endl << std::endl;
    }
}