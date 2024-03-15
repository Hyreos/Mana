#pragma once

#include "BinaryOp.hh"

#include <string>

namespace mana::ast {
    class ScopeResolutionOp : public BinaryOp {
    public:
        ScopeResolutionOp(std::unique_ptr<TreeNode> lhs, std::unique_ptr<TreeNode> rhs);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void accept(TreeVisitor* visitor) override;
    };
}