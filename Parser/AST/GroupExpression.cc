#include "GroupExpression.hh"

namespace mana::ast {
    GroupExpression::GroupExpression(const Expression* expression)
        : m_expression { expression }
    {
    }

    const GroupExpression* GroupExpression::clone(CloneContext& ctx) const
    {
        return ctx.create<GroupExpression>(ctx.clone(m_expression));
    }

    void GroupExpression::print(std::ostream& stream, size_t ident) const
    {
        stream << " ( ";

        m_expression->print(stream, ident);

        stream << " ) ";
    }
}