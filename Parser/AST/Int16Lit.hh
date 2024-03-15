#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

#include "Literal.hh"

namespace mana::ast {
    class Int16Lit : public Literal {
    public:
        Int16Lit(int16_t value);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void accept(TreeVisitor* visitor) override;
    private:
        int16_t m_value;
    };
}