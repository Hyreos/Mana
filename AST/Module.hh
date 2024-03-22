#pragma once

#include <vector>

#include "AST/Declaration.hh"

namespace mana::ast {
    class Module {
    public:
        void addDeclaration(const Declaration* declaration);

        const std::vector<const Declaration*>& getDeclarationList() const;
    private:
        std::vector<const Declaration*> m_globalDeclarations;
    };
}