#pragma once

#include "TreeNode.hh"

#include "Attribute.hh"
#include "TypeDeclaration.hh"
#include "IdentifierExpression.hh"

namespace mana::ast {
    class ComponentDeclaration final : public rtti::Castable<ComponentDeclaration,  TypeDeclaration> {
    public:
        ComponentDeclaration(
            const std::string& name,
            std::vector<const Declaration*> fields,
            std::vector<const IdentifierExpression*> inheritances,
            bool isExported,
            const std::vector<const Attribute*> attributes
        );

        const ComponentDeclaration* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;

        void setExportStatus(bool value);

        bool isExported() const;
    private:
        bool m_exported { false };

        std::vector<const Declaration*> m_fields;

        std::vector<const IdentifierExpression*> m_inheritances;

        std::vector<const Attribute*> m_attributes;
    };
}

MANA_RTTI_TYPE(mana::ast::ComponentDeclaration)