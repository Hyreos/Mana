#pragma once

#include "AST/Module.hh"

#include "AST/Type.hh"

#include <string>
#include <set>

namespace mana {
    struct TypeDependency {
        std::set<std::string> deps;
        const ast::Declaration* declaration = nullptr;
    };

    class DependencyGraph {
    public:
        DependencyGraph(const std::vector<const ast::Module*>& module_list);
    
        const std::vector<const ast::Type*> typeDependencies(const ast::Type* type);

        size_t transverseDependencies(const std::string& name, size_t depth = 0);
    private:
        std::vector<std::pair<size_t, const ast::Declaration*>> m_declv;

        std::unordered_map<std::string, TypeDependency> m_decl_deps;
    };
}