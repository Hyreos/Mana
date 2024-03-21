#pragma once

#include "Expression.hh"

#include <string>

namespace mana::ast {
    class IdentifierExpression final : public rtti::Castable<IdentifierExpression, Expression> {
    public:
        IdentifierExpression(const std::string& identifier);

        const IdentifierExpression* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;
    private:
        std::string m_identifier;
    };
}

