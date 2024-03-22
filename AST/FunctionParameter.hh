#pragma once

#include "Utils/RTTI.hh"
#include "Variable.hh"
#include "Type.hh"
#include "Attribute.hh"
#include "Qualifier.hh"
#include "IdentifierExpression.hh"
#include "Expression.hh"

namespace mana::ast {
    class FunctionParameter final : public rtti::Castable<FunctionParameter, Variable>
    {
    public:
        FunctionParameter(
            const std::vector<const Attribute*> attributes,
            const std::vector<const Qualifier*> qualifiers,
            const Type* type,
            const IdentifierExpression* identifier,
            const Expression* default_value_expression = nullptr
        );

        void print(std::ostream& stream, size_t ident) const override;

        const FunctionParameter* clone(CloneContext& ctx) const override;

        const Type* type() const;
    private:
        const std::vector<const Attribute*> m_attributes;
        const std::vector<const Qualifier*> m_qualifiers;
        const IdentifierExpression* m_identifier;
        const Type* m_type;
        const Expression* m_defaultValueExpression;
    };
}