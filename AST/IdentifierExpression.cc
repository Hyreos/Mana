#include "IdentifierExpression.hh"

namespace mana::ast {
    IdentifierExpression::IdentifierExpression(const std::string& identifier) 
        : m_identifier { identifier }
    {
    }

    const IdentifierExpression* IdentifierExpression::clone(CloneContext& ctx) const
    {
        return ctx.create<IdentifierExpression>(m_identifier);
    }

    void IdentifierExpression::print(std::ostream& stream, size_t ident) const
    {
        TreeNode::print(stream, ident);

        stream << m_identifier;
    }
}

MANA_RTTI_TYPE(mana::ast::IdentifierExpression);