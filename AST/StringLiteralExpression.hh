#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

#include "LiteralExpression.hh"

namespace mana::ast {
    class StringLiteralExpression final : public rtti::Castable<StringLiteralExpression, LiteralExpression>  {
    public:
        StringLiteralExpression(const std::string& str);

        const StringLiteralExpression* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;
    private:
        const std::string m_value;
    };
}

