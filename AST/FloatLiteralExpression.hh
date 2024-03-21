#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

#include "LiteralExpression.hh"

namespace mana::ast {
    class FloatLiteralExpression final : public rtti::Castable<FloatLiteralExpression, LiteralExpression>  {
    public:
        FloatLiteralExpression(double value);

        const FloatLiteralExpression* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;
    private:
        double m_value;
    };
}
