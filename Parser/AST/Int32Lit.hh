#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

#include "Literal.hh"

namespace mana::ast {
    class Int32Lit : public Literal {
    public:
        Int32Lit(int32_t value);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void accept(TreeVisitor* visitor) override;
    private:
        int32_t m_value;
    };
}