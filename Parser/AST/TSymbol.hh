#pragma once

#include "TreeNode.hh"

#include <string>

namespace mana {
    class TSymbol : public TreeNode {
    public:
        static constexpr TreeNode::Type kind { TreeNode::Type::kTSymbol };

        TSymbol(const std::string& identifier);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void accept(TreeVisitor* visitor) override;
    private:
        std::string m_identifier;
    };
}