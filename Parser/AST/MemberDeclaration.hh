#pragma once

#include "Declaration.hh"
#include "IdentifierExpression.hh"
#include "Attribute.hh"
#include "Qualifier.hh"

#include <optional>
#include <string>
#include <vector>

namespace mana::ast {
    class MemberDeclaration final : public rtti::Castable<MemberDeclaration, Declaration> {
    public:
        MemberDeclaration(
            const IdentifierExpression* type,
            const std::string& name,
            const Expression* defaultValue,
            bool isOptional,
            const std::vector<const Attribute*> attributes,
            const std::vector<const Qualifier*> qualifiers
        );

        const MemberDeclaration* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;

        const std::string& name() const;
    private:
        std::string m_name;
        bool m_optional;
        const IdentifierExpression* m_type;
        const Expression* m_default;
        const std::vector<const Attribute*> m_attributes;
        const std::vector<const Qualifier*> m_qualifiers;
    };
}

MANA_RTTI_TYPE(mana::ast::MemberDeclaration);