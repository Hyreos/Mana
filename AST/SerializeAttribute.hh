#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

#include "Attribute.hh"
#include "Expression.hh"

namespace mana::ast {
    class SerializeAttribute final : public rtti::Castable<SerializeAttribute, Attribute>  {
    public:
        SerializeAttribute(const Expression* expression);

        const SerializeAttribute* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;

        std::string name() const override;
    private:
        const Expression* m_expression;
    };
}

MANA_RTTI_TYPE(mana::ast::SerializeAttribute);