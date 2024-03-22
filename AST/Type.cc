#include "Type.hh"

namespace mana::ast {
    Type::Type(
        const IdentifierExpression* type_identifier,
        const Type* subtype
    ) : m_symbol { type_identifier }, m_subtype { subtype }
    {
    }

    void Type::print(std::ostream& stream, size_t ident) const
    {
        m_symbol->print(stream, ident);

        if (m_subtype) {
            stream << "<";

            m_subtype->print(stream, ident);

            stream << ">";
        }
    }

    const Type* Type::subType() const
    {
        return m_subtype;
    }

    const Type* Type::clone(CloneContext& ctx) const
    {
        return ctx.create<Type>(ctx.clone(m_symbol));
    }

    const IdentifierExpression* Type::symbol() const
    {
        return m_symbol;
    }
}

MANA_RTTI_TYPE(mana::ast::Type)