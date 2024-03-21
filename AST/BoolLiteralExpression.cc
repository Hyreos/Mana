#include "BoolLiteralExpression.hh"

namespace mana::ast {
    BoolLiteralExpression::BoolLiteralExpression(bool value) 
        : m_value { value }
    {
    }

    const BoolLiteralExpression* BoolLiteralExpression::clone(CloneContext& ctx) const
    {
        return ctx.create<BoolLiteralExpression>(m_value);
    }

    void BoolLiteralExpression::print(std::ostream& stream, size_t ident) const
    {
        TreeNode::print(stream, ident);

        stream << "(" << m_value << "_bool" << ")";
    }
}

MANA_RTTI_TYPE(mana::ast::BoolLiteralExpression);
