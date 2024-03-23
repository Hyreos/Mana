#pragma once

#include "AST/Module.hh"
#include "AST/ComponentDeclaration.hh"
#include "AST/AliasDeclaration.hh"
#include "AST/ImportDeclaration.hh"
#include "AST/Variable.hh"

namespace mana {
    class Compiler;

    class Resolver {
    public:
        Resolver(Compiler* compiler);

        void resolve(ast::Module* module);

        void resolve(ast::ComponentDeclaration* component);

        void resolve(ast::AliasDeclaration* alias);

        void resolve(ast::ImportDeclaration* import_declaration);

        void resolve(ast::Type* type);

        void resolve(ast::Variable* variable);
        
        void resolve(ast::IdentifierExpression* identifier);
    private:
        Compiler* m_compiler; 
    };
}