#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

namespace mana {
    class Number : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kNumber };

        Number(std::variant<double, int64_t> value);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        std::variant<double, int64_t> m_value;
    };
}