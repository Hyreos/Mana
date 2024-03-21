#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

#include "Attribute.hh"
#include "Expression.hh"

namespace mana::ast {
    class ExposedAttribute final : public rtti::Castable<ExposedAttribute, Attribute>  {
    public:
        ExposedAttribute(
            const std::vector<const Expression*> expression
        );

        const ExposedAttribute* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;

        std::string name() const override;
    private:
        const std::vector<const Expression*> m_expression;
    };
}

