#include "DependencyGraph.hh"
#include "AST/ComponentDeclaration.hh"
#include "AST/EnumDeclaration.hh"
#include "AST/MemberDeclaration.hh"
#include "AST/FunctionDeclaration.hh"

#include <vector>
#include <algorithm>
#include <set>

namespace mana {
    struct DependencyNode {
        ast::TreeNode* declaration;

        std::vector<const DependencyNode*> dependencies;
    };

    const std::vector<const ast::Type*> DependencyGraph::typeDependencies(
        const ast::Type* type
    )
    {

    }

    DependencyGraph::DependencyGraph(const std::vector<const ast::Module*>& module_list)
    {
        std::unordered_map<const ast::Declaration*, TypeDependency> decl_deps;

        for (const auto* mod : module_list) {
            auto& declaration_list = mod->getDeclarationList();

            for (const auto* decl : declaration_list) {
                if (decl->is<ast::ComponentDeclaration>()) {
                    auto add_type_to_decl = [&](const ast::Type* type) {
                        if (type)
                            for (auto* ttype = type; ttype->subType(); ttype = ttype->subType())
                                decl_deps[decl].deps.emplace(ttype->symbol());
                    };
                        

                    auto component_decl = decl->as<ast::ComponentDeclaration>();

                    for (auto* member : component_decl->members()) {
                        rtti::Match(
                            member, 
                            [&](const ast::FunctionDeclaration* fn) {
                                for (auto& arg : fn->args())
                                    add_type_to_decl(arg->type());

                                add_type_to_decl(fn->returnType());
                            }, 
                            [&](const ast::MemberDeclaration* member) {
                                add_type_to_decl(member->type());
                            },  
                            [](Default) {
                                std::cerr << "invalid node type received as global declaration!" << std::endl;
                            });
                    }
                }
            }
        }

        
    }
}