#pragma once

#include "AST/Declaration.hh"

namespace mana::sem {
    class Module {
    public:
        Module() = default;

        ~Module() = default;

        void addGlobalDeclaration(const ast::Declaration* declaration);
    private:
        std::vector<const ast::Declaration*> m_globalDeclarations;
    };
}