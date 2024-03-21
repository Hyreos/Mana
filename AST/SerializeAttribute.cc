#include "SerializeAttribute.hh"

namespace mana::ast {
    SerializeAttribute::SerializeAttribute(const Expression* expression)
        : m_expression { expression }
    {
    }

    const SerializeAttribute* SerializeAttribute::clone(CloneContext& ctx) const
    {
        return ctx.create<SerializeAttribute>(ctx.clone(m_expression));
    }

    std::string SerializeAttribute::name() const
    {
        return "serialize";
    }

    void SerializeAttribute::print(std::ostream& stream, size_t ident) const
    {
        stream << "@" << name() << "(";
        
        m_expression->print(stream, ident);

        stream << ")";
    }
}

MANA_RTTI_TYPE(mana::ast::SerializeAttribute);