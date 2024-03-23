#include "Module.hh"

namespace mana::ast {
    void Module::addDeclaration(const Declaration* declaration)
    {
        m_globalDeclarations.push_back(declaration);
    }

    const std::vector<const Declaration*>& Module::declarations() const
    {
        return m_globalDeclarations;
    }
}