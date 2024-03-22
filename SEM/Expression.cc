#include "Expression.hh"

namespace mana::sem {
    Expression::Expression(const ast::Expression* expression) : 
        m_expression { expression }
    {
    } 

    const ast::Expression* Expression::expression() const
    {
        return m_expression;
    }
}

MANA_RTTI_TYPE(mana::sem::Expression);