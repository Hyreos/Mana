#include "Resolver.hh"

#include "DependencyGraph.hh"

namespace mana {
    Resolver::Resolver(const std::vector<const ast::Module*> list_of_modules) 
        : m_moduleList { list_of_modules }
    {
    }

    void Resolver::resolve()
    {
        DependencyGraph graph(m_moduleList);
    }
}