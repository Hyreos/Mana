#include "FunctionParameter.hh"

namespace mana::ast {
    FunctionParameter::FunctionParameter(
        const std::vector<const Attribute*> attributes,
        const std::vector<const Qualifier*> qualifiers,
        const Type* type,
        const IdentifierExpression* identifier,
        const Expression* default_value_expression
    ) : m_attributes { attributes }, 
        m_type { type }, 
        m_identifier { identifier },
        m_defaultValueExpression { default_value_expression },
        m_qualifiers { qualifiers }
    {
    }

    void FunctionParameter::print(std::ostream& stream, size_t ident) const
    {
        for (auto& attribute : m_attributes) {
            attribute->print(stream, ident);

            stream << " ";
        }

        for (auto& qualifier : m_qualifiers) {
            qualifier->print(stream, ident);

            stream << " ";
        }

        m_type->print(stream, ident);

        stream << " ";

        m_identifier->print(stream, ident);

        if (m_defaultValueExpression) {
            stream << " = ";

            m_defaultValueExpression->print(stream, ident);
        }
    }

    const FunctionParameter* FunctionParameter::clone(CloneContext& ctx) const
    {
        return ctx.create<FunctionParameter>(
            ctx.clone(m_attributes),
            ctx.clone(m_qualifiers),
            ctx.clone(m_type),
            ctx.clone(m_identifier),
            (m_defaultValueExpression) ? ctx.clone(m_defaultValueExpression) : nullptr
        );
    }
}

MANA_RTTI_TYPE(mana::ast::FunctionParameter);