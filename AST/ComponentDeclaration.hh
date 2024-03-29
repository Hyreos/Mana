#pragma once

#include "TreeNode.hh"

#include "Attribute.hh"
#include "TypeDeclaration.hh"
#include "Qualifier.hh"
#include "IdentifierExpression.hh"

namespace mana::ast {
    class ComponentDeclaration final : public rtti::Castable<ComponentDeclaration,  TypeDeclaration> {
    public:
        ComponentDeclaration(
            const std::string& name,
            std::vector<const Declaration*> fields,
            std::vector<const IdentifierExpression*> inheritances,
            std::vector<const Qualifier*> qualfiiers,
            const std::vector<const Attribute*> attributes
        );

        const ComponentDeclaration* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;

        const std::vector<const IdentifierExpression*>& inheritances() const;

        const std::vector<const Declaration*>& members() const;

        const std::string& name() const;
    private:
        bool m_exported { false };

        std::vector<const Qualifier*> m_qualifiers;

        std::vector<const Declaration*> m_fields;

        std::vector<const IdentifierExpression*> m_inheritances;

        std::vector<const Attribute*> m_attributes;
    };
}

