#pragma once

#include "Declaration.hh"
#include "IdentifierExpression.hh"

#include <optional>
#include <string>

namespace mana::ast {
    class MemberDeclaration final : public rtti::Castable<MemberDeclaration, Declaration> {
    public:
        MemberDeclaration(
            const IdentifierExpression* type,
            const std::string& name,
            const Expression* defaultValue,
            bool isOptional
        );

        const MemberDeclaration* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;

        const std::string& name() const;
    private:
        std::string m_name;
        bool m_optional;
        const IdentifierExpression* m_type;
        const Expression* m_default;
    };
}

MANA_RTTI_TYPE(mana::ast::MemberDeclaration);