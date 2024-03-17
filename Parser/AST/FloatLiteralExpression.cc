#include "FloatLiteralExpression.hh"

namespace mana::ast {
    FloatLiteralExpression::FloatLiteralExpression(double value) 
        : m_value { value }
    {
    }

    const FloatLiteralExpression* FloatLiteralExpression::clone(CloneContext& ctx) const
    {
        return ctx.create<FloatLiteralExpression>(m_value);
    }

    void FloatLiteralExpression::print(std::ostream& stream, size_t ident) const
    {
        TreeNode::print(stream, ident);

        stream << "(" << m_value << "_fp" << ")";
    }
}