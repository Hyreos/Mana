#include "FunctionDeclaration.hh"

namespace mana::ast {
    FunctionDeclaration::FunctionDeclaration(
        const Type* type,
        const IdentifierExpression* identifier,
        const std::vector<const Attribute*> attributes,
        const std::vector<const Qualifier*> qualifiers,
        const std::vector<const FunctionParameter*> args_list
    ) : m_type { type }, 
        m_identifier { identifier }, 
        m_attributes { attributes },
        m_argsList { args_list },
        m_qualifiers { qualifiers }
    {
    }

    const FunctionDeclaration* FunctionDeclaration::clone(CloneContext& ctx) const
    {
        return ctx.create<FunctionDeclaration>(
            ctx.clone(m_type),
            ctx.clone(m_identifier),
            ctx.clone(m_attributes),
            ctx.clone(m_qualifiers),
            ctx.clone(m_argsList)
        );
    }

    void FunctionDeclaration::print(std::ostream& stream, size_t ident) const
    {
        for (const auto* attr : m_attributes) {
            attr->print(stream, ident);

            stream << " ";
        }

        for (const auto* qualifier : m_qualifiers) {
            qualifier->print(stream, ident);

            stream << " ";
        }

        stream << "fn ";

        m_identifier->print(stream, ident);

        stream << "(";

        for (auto i = 0; i < m_argsList.size(); i++) {
            if (i > 0) stream << ", ";

            m_argsList[i]->print(stream, ident);
        }

        stream << ") -> ";

        m_type->print(stream, ident);
    }

    const IdentifierExpression* FunctionDeclaration::identifier() const
    {
        return m_identifier;
    }

    const Type* FunctionDeclaration::returnType() const
    {
        return m_type;
    }

    const std::vector<const FunctionParameter*>& FunctionDeclaration::args() const
    {
        return m_argsList;
    }
}

MANA_RTTI_TYPE(mana::ast::FunctionDeclaration)