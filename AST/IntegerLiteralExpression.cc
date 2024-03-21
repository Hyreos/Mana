#include "IntegerLiteralExpression.hh"

namespace mana::ast {
    IntegerLiteralExpression::IntegerLiteralExpression(
        const Sign sign, 
        uint64_t value
    ) : m_sign { sign }, m_value { value }
    {

    }

    const IntegerLiteralExpression* IntegerLiteralExpression::clone(CloneContext& ctx) const
    {
        return ctx.create<IntegerLiteralExpression>(
            m_sign,
            m_value
        );
    }

    void IntegerLiteralExpression::print(std::ostream& stream, size_t ident) const
    {
        TreeNode::print(stream, ident);

        if (m_sign == Sign::kUnsigned) stream << static_cast<uint64_t>(m_value);
        else stream << m_value;
    }
}