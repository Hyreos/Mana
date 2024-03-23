#pragma once

#include "Node.hh"
#include "AST/ComponentDeclaration.hh"

namespace mana::sem {
    class Component : public rtti::Castable<Component, Node>
    {
    public:
        ast::ComponentDeclaration* ast();
    private:
        ast::ComponentDeclaration* m_ast;
    };
}