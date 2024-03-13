#pragma once

#include "TreeNode.hh"

#include <string>

namespace mana {
    class CompFieldDecl : public TreeNode {
    public:
        static constexpr TreeNode::Type kind { TreeNode::Type::kComponentField };

        CompFieldDecl(
            std::unique_ptr<TreeNode> type,
            const std::string& name
        );

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        const std::string& name() const;
    private:
        std::string m_name;
        std::unique_ptr<TreeNode> m_type;
    };
}