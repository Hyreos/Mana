#pragma once

#include "Module.hh"

#include "AST/Expression.hh"
#include "Expression.hh"

namespace mana {
    class Resolver {
    public:
        Resolver(const sem::Module* module);

        const sem::Module* root() const;
    private:
        std::unique_ptr<sem::Expression> expression(const ast::Expression* expression);

        const sem::Module* m_module;
    };
}