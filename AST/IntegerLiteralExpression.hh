#pragma once

#include "LiteralExpression.hh"

namespace mana::ast {
    class IntegerLiteralExpression final : public rtti::Castable<IntegerLiteralExpression, LiteralExpression> {
    public:
        enum class Sign {
            kSigned,
            kUnsigned
        };

        IntegerLiteralExpression(const Sign sign, uint64_t value);
    
        const IntegerLiteralExpression* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;
    private:
        Sign m_sign;

        uint64_t m_value;
    };
}

MANA_RTTI_TYPE(mana::ast::IntegerLiteralExpression);
