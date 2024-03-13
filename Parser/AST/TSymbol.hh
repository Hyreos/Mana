#pragma once

#include "TreeNode.hh"

#include <string>

namespace mana {
    class TSymbol : public TreeNode {
    public:
        static constexpr TreeNode::Type kind { TreeNode::Type::kTSymbol };

        TSymbol(const std::string& identifier, bool isOptional);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        std::string m_identifier;
        bool m_isOptional;
    };
}