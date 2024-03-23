#include "DependencyGraph.hh"
#include "AST/ComponentDeclaration.hh"
#include "AST/EnumDeclaration.hh"
#include "AST/MemberDeclaration.hh"
#include "AST/AliasDeclaration.hh"
#include "AST/FunctionDeclaration.hh"
#include "AST/ImportDeclaration.hh"

#include <vector>
#include <algorithm>
#include <set>

namespace mana {
    const std::vector<const ast::Type*> DependencyGraph::typeDependencies(
        const ast::Type* type
    )
    {
    }

    DependencyGraph::DependencyGraph(ast::Module* mod)
    {
        // Iterate over modules and make a dependency list for each one of them.
        auto& declaration_list = mod->declarations();

        for (const auto* decl : declaration_list) {
            auto add_type_to_decl = [&](const std::string target, const ast::Type* type) -> void {
                for (auto* ttype = type; ttype; ttype = ttype->subType()) {
                    m_decl_deps[target].deps.emplace(ttype->symbol()->identifier());
                    m_decl_deps[target].declaration = decl;
                }
            };

            Match(
                decl,
                [&](const ast::ComponentDeclaration* component_decl) {
                    for (auto* member : component_decl->members()) {
                        m_decl_deps[component_decl->name()].declaration = component_decl;

                        Match(
                            member, 
                            [&](const ast::FunctionDeclaration* fn) {
                                for (auto& arg : fn->args())
                                    add_type_to_decl(component_decl->name(), arg->type());

                                add_type_to_decl(component_decl->name(), fn->returnType());
                            }, 
                            [&](const ast::MemberDeclaration* member) {
                                add_type_to_decl(component_decl->name(),  member->type());
                            },
                            [](Default) {
                                std::cerr << "invalid node type received as global declaration!" << std::endl;
                            });
                    }
                },
                [&](const ast::AliasDeclaration* alias_decl) {
                    m_decl_deps[alias_decl->identifier()->identifier()].declaration = alias_decl;
                },
                [](Default) {
                    std::cerr << "Trying to match wrong type!" << std::endl;
                }
            );
        }

        for (auto& [symbol, dependencies] : m_decl_deps)
            // Transverse dependencies 
            transverseDependencies(symbol);

        std::sort(m_declv.begin(), m_declv.end(), [](auto& a, auto& b) {
            return (a.first > b.first);
        });

        for (auto i = 0; i < m_declv.size(); i++) {
            for (auto j = i + 1; j < m_declv.size(); j++) {
                if (m_declv[j].second == m_declv[i].second) {
                    m_declv.erase(m_declv.begin() + j);
                    
                    j--;
                }
            }
        }

        auto& module_decls = const_cast<std::vector<const ast::Declaration*>&>(mod->declarations());

        module_decls.clear();

        for (auto& [_, m] : m_declv) {
            m->print(std::cout, 0);
            
            module_decls.push_back(m);
        }
    }

    size_t DependencyGraph::transverseDependencies(const std::string& name, size_t depth)
    {
        // TODO: Check for missing types
        auto it = m_decl_deps.find(name);

        // This ensures that only dependencies with a declaration will be added later in this function. 
        if (it == m_decl_deps.end()) return true;

        for (auto& dep : it->second.deps) {
            transverseDependencies(dep, depth + 1);
        }

        m_declv.push_back({ depth, it->second.declaration });

        return depth;
    }
}