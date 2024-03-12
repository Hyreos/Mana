#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

namespace mana {
    class Int16Lit : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kInt16Lit };

        Int16Lit(int16_t value);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        int16_t m_value;
    };
}