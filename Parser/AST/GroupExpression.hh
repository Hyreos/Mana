#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

#include "Expression.hh"

namespace mana::ast {
    class GroupExpression final : public rtti::Castable<GroupExpression, Expression>  {
    public:
        GroupExpression(const Expression* expression);

        const GroupExpression* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;
    private:
        const Expression* m_expression;
    };
}

MANA_RTTI_TYPE(mana::ast::GroupExpression);