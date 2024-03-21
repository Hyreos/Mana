#include "EntryDeclaration.hh"

namespace mana::ast {
    EEntryDeclaration::EEntryDeclaration(
        const std::string& name,
        const Expression* expr,
        const std::vector<const Attribute*> attributes
    ) : m_name { name },
        m_attributes { attributes },
        m_expression { expr }
    {
    }

    const EEntryDeclaration* EEntryDeclaration::clone(CloneContext& ctx) const
    {
        return ctx.create<EEntryDeclaration>(m_name, ctx.clone(m_expression), ctx.clone(m_attributes));
    }

    void EEntryDeclaration::print(std::ostream& stream, size_t ident) const
    {
        for (auto& attr : m_attributes) {
            attr->print(stream, ident);
            
            stream << " ";
        }

        stream << m_name << " = ";

        m_expression->print(stream, ident);
    }
}

MANA_RTTI_TYPE(mana::ast::EEntryDeclaration)