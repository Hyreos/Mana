#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

#include "Attribute.hh"

namespace mana::ast {
    class CCAttribute final : public rtti::Castable<CCAttribute, Attribute>  {
    public:
        CCAttribute() = default;

        const CCAttribute* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;

        std::string name() const override;
    };
}
