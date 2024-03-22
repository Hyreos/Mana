#pragma once

#include "TreeNode.hh"

#include "Attribute.hh"
#include "TypeDeclaration.hh"
#include "IdentifierExpression.hh"
#include "Qualifier.hh"
#include "EntryDeclaration.hh"
#include "Type.hh"

namespace mana::ast {
    class AliasDeclaration final : public rtti::Castable<AliasDeclaration,  TypeDeclaration> {
    public:
        AliasDeclaration(
            const ast::IdentifierExpression* identifier,
            const std::vector<const Type*>& types,
            const std::vector<const Attribute*>& attributes
        );

        const AliasDeclaration* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;

        const ast::IdentifierExpression* identifier() const;
    private:
        const ast::IdentifierExpression* m_identifier;
        const std::vector<const Type*> m_types;
        const std::vector<const Attribute*> m_attributes;
    };
}

