#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

namespace mana {
    class Uint16Lit : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kUint16Lit };

        Uint16Lit(uint16_t value);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        uint16_t m_value;
    };
}