#include "Module.hh"

namespace mana::ast {
    void Module::addDeclaration(Declaration* declaration)
    {
        m_globalDeclarations.push_back(declaration);
    }

    std::vector<const Declaration*>& Module::getDeclarationList() const
    {
        return m_globalDeclarations;
    }
}