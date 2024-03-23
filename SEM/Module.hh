#pragma once

#include "Node.hh"

#include "AST/Module.hh"

namespace mana::sem {
    class Module : public rtti::Castable<Module, Node> {
    public:
        Module(ast::Module* mod);

        ~Module() = default;
    
        ast::Module* ast();
    private:
        ast::Module* m_module;
    };
}