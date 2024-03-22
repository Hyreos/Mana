#pragma once

#include <vector>

#include "AST/Declaration.hh"

namespace mana::ast {
    class Module {
    public:
        void addDeclaration(Declaration* declaration);

        const std::vector<Declaration*>& getDeclarationList() const;
    private:
        std::vector<Declaration*> m_globalDeclarations;
    };
}