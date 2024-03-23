#include "Resolver.hh"

#include "DependencyGraph.hh"

#include "../Compiler.hh"

namespace mana {
    Resolver::Resolver(Compiler* compiler) 
        : m_compiler { compiler }
    {
    }

    void Resolver::resolve(ast::Module* module)
    {
        for (auto& module : module->declarations()) {
            Match(
                module,
                [&](const ast::ImportDeclaration* declaration)
                {
                    resolve(const_cast<ast::ImportDeclaration*>(declaration));
                },
                [](Default) {}
            );    
        }

        for (auto& module : module->declarations()) {
            Match(
                module,
                [&](const ast::AliasDeclaration* declaration)
                {
                    resolve(const_cast<ast::AliasDeclaration*>(declaration));
                },
                [&](const ast::ComponentDeclaration* declaration)
                {
                    resolve(const_cast<ast::ComponentDeclaration*>(declaration));
                },
                [](Default) {}
            );
        }
    }

    void Resolver::resolve(ast::Type* type)
    {
        // resolve type

    }

    void Resolver::resolve(ast::AliasDeclaration* alias)
    {
        // resolve alias

    }

    void Resolver::resolve(ast::ComponentDeclaration* component)
    {
        // resolve component declaration
                
    }

    void Resolver::resolve(ast::Variable* variable)
    {
        // resolve a variable

    }

    void Resolver::resolve(ast::IdentifierExpression* expression)
    {
        // resolve an identifier
    }

    void Resolver::resolve(ast::ImportDeclaration* import_declaration)
    {
        // First load the imported module
        m_compiler->loadModule(import_declaration->path());
    }
}