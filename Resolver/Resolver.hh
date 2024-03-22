#pragma once

#include "AST/Module.hh"

namespace mana {
    class Resolver {
    public:
        Resolver(const std::vector<const ast::Module*> list_of_modules);

        void resolve();
    private:
        const std::vector<const ast::Module*> m_moduleList;
    };
}