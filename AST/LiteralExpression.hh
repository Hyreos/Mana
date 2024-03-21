#pragma once

#include "Expression.hh"

#include <string>
#include <variant>

namespace mana::ast {
    class LiteralExpression : public rtti::Castable<LiteralExpression, Expression> {
    public:
        LiteralExpression() = default;
    };
}

