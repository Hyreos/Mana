#include "AliasDeclaration.hh"

namespace mana::ast {
    AliasDeclaration::AliasDeclaration(
        const ast::IdentifierExpression* identifier,
        const std::vector<const Type*>& types,
        const std::vector<const Attribute*>& attributes
    )
        : m_types{ types },
            m_attributes { attributes },
            m_identifier { identifier }
    {
    }

    const AliasDeclaration* AliasDeclaration::clone(CloneContext& ctx) const
    {
        return ctx.create<AliasDeclaration>(
            ctx.clone(m_identifier), 
            ctx.clone(m_types), 
            ctx.clone(m_attributes)
        );
    }

    const ast::IdentifierExpression* AliasDeclaration::identifier() const
    {
        return m_identifier;
    }

    void AliasDeclaration::print(std::ostream& stream, size_t ident) const
    {
        for (auto& attr : m_attributes) {
            attr->print(stream, ident);
            
            stream << " ";
        }

        stream << "alias ";
        
        m_identifier->print(stream, ident);
        
        stream << " = (" << std::endl;

        size_t i = 0;

        for (auto& type : m_types) {
            if (i++ > 0) stream << " | ";

            std::cout << std::string(2 * ident + 2, ' ');
            
            type->print(stream, ident + 1);
        }

        stream << std::endl;
        
        stream << ");" << std::endl << std::endl;
    }
}

MANA_RTTI_TYPE(mana::ast::AliasDeclaration)