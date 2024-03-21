#include "StringLiteralExpression.hh"

namespace mana::ast {
    StringLiteralExpression::StringLiteralExpression(
        const std::string& str
    ) : m_value { str }
    {
    }

    const StringLiteralExpression* StringLiteralExpression::clone(CloneContext& ctx) const
    {
        return ctx.create<StringLiteralExpression>(m_value);
    }

    void StringLiteralExpression::print(std::ostream& stream, size_t ident) const
    {
        TreeNode::print(stream, ident);

        stream << "(\"" << m_value << "\"_string" << ")";
    }
}

MANA_RTTI_TYPE(mana::ast::StringLiteralExpression);