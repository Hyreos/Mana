#pragma once

#include "TreeNode.hh"

#include "Attribute.hh"
#include "TypeDeclaration.hh"
#include "IdentifierExpression.hh"
#include "Qualifier.hh"
#include "EntryDeclaration.hh"

namespace mana::ast {
    class EnumDeclaration final : public rtti::Castable<EnumDeclaration,  TypeDeclaration> {
    public:
        EnumDeclaration(
            const std::string& name,
            const std::vector<const EEntryDeclaration*>& entries,
            const std::vector<const Attribute*>& attributes
        );

        const EnumDeclaration* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;
    private:
        const std::string m_name;
        const std::vector<const EEntryDeclaration*> m_entries;
        const std::vector<const Attribute*> m_attributes;
    };
}

MANA_RTTI_TYPE(mana::ast::EnumDeclaration)