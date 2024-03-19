#pragma once

#include "Expression.hh"

#include "Utils/RTTI.hh"

namespace mana::ast {
    class UnaryExpression final : public rtti::Castable<UnaryExpression, Expression> {
    public:
        enum class Kind {
            kMinus, kPlus, kInv
        };

        UnaryExpression(Kind kind, const Expression* operand);

        Kind kind() const;

        const Expression* operand() const;

        const UnaryExpression* clone(CloneContext& ctx) const override;
    private:
        Kind m_kind;
        const Expression* m_operand;
    };
}

MANA_RTTI_TYPE(mana::ast::UnaryExpression);