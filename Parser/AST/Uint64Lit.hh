#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

#include "Literal.hh"

namespace mana::ast {
    class Uint64Lit : public Literal {
    public:
        Uint64Lit(uint64_t value);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void accept(TreeVisitor* visitor) override;
    private:
        uint64_t m_value;
    };
}