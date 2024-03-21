#pragma once

#include <string>
#include <variant>
#include <memory>

#include "Expression.hh"
#include "Utils/RTTI.hh"

namespace mana::ast {
    class BinaryExpression final : public rtti::Castable<BinaryExpression, Expression> {
    public:
        enum class OpType {
            kAdd,
            kSubtract,
            kMultiply,
            kDivide,
            kModulus,
            kScopeResolution,
            kComma
        };

        BinaryExpression(
            const Expression* lhs,
            OpType type,
            const Expression* rhs
        );

        const Expression* lhs();

        const Expression* rhs();

        OpType type() const;

        void print(std::ostream& stream, size_t ident) const override;

        const BinaryExpression* clone(CloneContext&) const override;
    protected:        
        OpType m_type;
        const Expression* m_lhs;
        const Expression* m_rhs;
    };
}

