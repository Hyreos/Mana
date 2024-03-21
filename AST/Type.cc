#include "Type.hh"

namespace mana::ast {
    Type::Type(const IdentifierExpression* type_identifier) : m_symbol { type_identifier }
    {
    }

    void Type::print(std::ostream& stream, size_t ident) const
    {
        m_symbol->print(stream, ident);
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