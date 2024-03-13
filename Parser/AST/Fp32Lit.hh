#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

namespace mana {
    class Fp32Lit : public TreeNode {
    public:
        static constexpr TreeNode::Type kind { TreeNode::Type::kFp32Lit };

        Fp32Lit(float value);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        float m_value;
    };
}