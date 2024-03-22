#pragma once

#include "AST/Module.hh"

#include "AST/Type.hh"

#include <set>

namespace mana {
    struct TypeDependency {
        bool resolved = false;
        std::set<const ast::IdentifierExpression*> deps;
    };

    class DependencyGraph {
    public:
        DependencyGraph(const std::vector<const ast::Module*>& module_list);
    
        const std::vector<const ast::Type*> typeDependencies(const ast::Type* type);
    };
}