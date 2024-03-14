#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

#include "Literal.hh"

namespace mana {
    class Uint32Lit : public Literal {
    public:
        Uint32Lit(uint32_t value);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void accept(TreeVisitor* visitor) override;
    private:
        uint32_t m_value;
    };
}