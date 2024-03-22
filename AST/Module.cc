#include "Module.hh"

namespace mana::ast {
    void Module::addDeclaration(const Declaration* declaration)
    {
        m_globalDeclarations.push_back(declaration);
    }

    const std::vector<const Declaration*>& Module::getDeclarationList() const
    {
        return m_globalDeclarations;
    }
}