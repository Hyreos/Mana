#pragma once

#include "Unary.hh"

#include <string>

namespace mana::ast {
    class UnaryPlus : public Unary {
    public:
        UnaryPlus(std::unique_ptr<TreeNode> operand);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void accept(TreeVisitor* visitor) override;
    private:
        std::unique_ptr<TreeNode> m_operand;;
    };
}