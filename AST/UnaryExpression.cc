#include "UnaryExpression.hh"

namespace mana::ast {
    UnaryExpression::UnaryExpression(
        Kind kind,
        const Expression* operand
    ) : m_kind{ kind }, 
        m_operand { operand }
    {
    }

    UnaryExpression::Kind UnaryExpression::kind() const
    {
        return m_kind;
    }

    const Expression* UnaryExpression::operand() const
    {
        return &*m_operand;
    }

    const UnaryExpression* UnaryExpression::clone(CloneContext& ctx) const
    {
        return ctx.create<UnaryExpression>(m_kind, ctx.clone(m_operand));
    }
}

MANA_RTTI_TYPE(mana::ast::UnaryExpression);