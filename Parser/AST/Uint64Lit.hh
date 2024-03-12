#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

namespace mana {
    class Uint64Lit : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kUint64Lit };

        Uint64Lit(uint64_t value);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        uint64_t m_value;
    };
}