#include "ExposedAttribute.hh"

namespace mana::ast {
    ExposedAttribute::ExposedAttribute(
        const std::vector<const Expression*> expression
    )
        : m_expression { expression }
    {
    }

    const ExposedAttribute* ExposedAttribute::clone(CloneContext& ctx) const
    {
        return ctx.create<ExposedAttribute>(ctx.clone(m_expression));
    }

    std::string ExposedAttribute::name() const
    {
        return "exposed";
    }

    void ExposedAttribute::print(std::ostream& stream, size_t ident) const
    {
        stream << "@" << name() << "(";

        for (auto i = 0; i < m_expression.size(); i++) {
            if (i > 0) stream << ", ";

            m_expression[i]->print(stream, ident);
        }

        stream << ")";
    }
}

MANA_RTTI_TYPE(mana::ast::ExposedAttribute);