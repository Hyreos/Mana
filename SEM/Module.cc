#include "Module.hh"

namespace mana::sem {
    void Module::addGlobalDeclaration(const ast::Declaration* declaration)
    {
        m_globalDeclarations.push_back(declaration);
    }
}