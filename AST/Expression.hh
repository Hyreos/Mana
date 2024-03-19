#pragma once

#include "TreeNode.hh"

#include "Utils/RTTI.hh"

#include <string>

namespace mana::ast {
    class Expression : public rtti::Castable<Expression, TreeNode> {
    public:
        Expression() = default;
    };
}

MANA_RTTI_TYPE(mana::ast::Expression);
