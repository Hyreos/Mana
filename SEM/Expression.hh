#pragma once

#include "Node.hh"

#include "AST/Expression.hh"

namespace mana::sem {
    class Expression : public rtti::Castable<Expression, Node> {
    public:
        Expression(const ast::Expression* expression);

        const ast::Expression* expression() const;
    private:
        const ast::Expression* m_expression;
    };
}