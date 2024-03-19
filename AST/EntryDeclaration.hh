#pragma once

#include "TreeNode.hh"

#include <string>
#include "Attribute.hh"
#include "Declaration.hh"
#include "Expression.hh"

namespace mana::ast {
    class EEntryDeclaration final : public rtti::Castable<EEntryDeclaration,  Declaration> {
    public:
        EEntryDeclaration(
            const std::string& name,
            const Expression* expr,
            const std::vector<const Attribute*> attributes
        );

        const EEntryDeclaration* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;
    private:
        const std::string m_name;
        const Expression* m_expression;
        const std::vector<const Attribute*> m_attributes;
    };
}

MANA_RTTI_TYPE(mana::ast::EEntryDeclaration)