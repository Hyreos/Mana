#pragma once

#include "TreeNode.hh"

namespace mana {
    class CompDecl : public TreeNode {
    public:
        static constexpr TreeNode::Type kind { TreeNode::Type::kComponent };

        CompDecl(
            const std::string& name,
            std::vector<std::unique_ptr<TreeNode>> fields,
            std::vector<std::unique_ptr<TreeNode>> inheritances
        );

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void setExportStatus(bool value);

        bool isExported() const;

        void accept(TreeVisitor* visitor) override;
    private:
        bool m_exported { false };

        std::vector<std::unique_ptr<TreeNode>> m_fields;

        std::vector<std::unique_ptr<TreeNode>> m_inheritances;

        std::string m_name;
    };
}