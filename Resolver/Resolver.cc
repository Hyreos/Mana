#include "Resolver.hh"

#include "DependencyGraph.hh"

#include "../Compiler.hh"

namespace mana {
    Resolver::Resolver(Compiler* compiler) 
        : m_compiler { compiler },
            m_module { nullptr }
    {
    }

    void Resolver::resolve(ast::Module* module)
    {
        m_module = module;

        for (auto i = 0; i < module->declarations().size(); i++) {
            Match(
                module->declarations()[i],
                [&](const ast::ImportDeclaration* declaration)
                {
                    resolve(const_cast<ast::ImportDeclaration*>(declaration));
                },
                [](Default) {}
            );
        }

        DependencyGraph depGraph(module);
        // all declarations are sorted at this point.

        for (auto& decl : module->declarations()) {
            Match(
                decl,
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
        auto* mod = m_compiler->loadModule(import_declaration->path());

        std::cout << "Adding " << mod->declarations().size() << " modules." << std::endl;
    
        for (auto& decl : mod->declarations()) {
            m_module->addDeclaration(decl);
        }

        std::cout << "Done!" << std::endl;
    }
}