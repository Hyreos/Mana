#pragma once

#include "TreeNode.hh"
#include "Type.hh"
#include "Attribute.hh"
#include "Declaration.hh"
#include "Expression.hh"
#include "Qualifier.hh"
#include "FunctionParameter.hh"

namespace mana::ast {
    class FunctionDeclaration final : 
        public rtti::Castable<FunctionDeclaration, Declaration> {
    public:
        FunctionDeclaration(
            const Type* type,
            const IdentifierExpression* identifier,
            const std::vector<const Attribute*> attributes,
            const std::vector<const Qualifier*> qualifiers,
            const std::vector<const FunctionParameter*> args_list
        );

        const FunctionDeclaration* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;

        const IdentifierExpression* identifier() const;

        const Type* returnType() const;

        const std::vector<const FunctionParameter*>& args() const;
    private:
        const Type* m_type;
        const IdentifierExpression* m_identifier;
        const std::vector<const Attribute*> m_attributes;
        const std::vector<const FunctionParameter*> m_argsList;
        const std::vector<const Qualifier*> m_qualifiers;
    };
}