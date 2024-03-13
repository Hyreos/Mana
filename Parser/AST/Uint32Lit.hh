#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

namespace mana {
    class Uint32Lit : public TreeNode {
    public:
        static constexpr TreeNode::Type kind { TreeNode::Type::kUint32Lit };

        Uint32Lit(uint32_t value);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        uint32_t m_value;
    };
}