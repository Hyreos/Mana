#pragma once

#include "TreeNode.hh"

namespace mana {
    class CompDecl : public TreeNode {
    public:
        static constexpr TreeNode::Type kind { TreeNode::Type::kComponent };

        CompDecl(
            const std::string& name,
            std::vector<std::unique_ptr<TreeNode>> fields,
            const std::vector<std::string>& inheritances,
            const std::vector<std::string>& cpp_inheritances
        );

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void setExportStatus(bool value);

        bool isExported() const;
    private:
        bool m_exported { false };

        std::vector<std::unique_ptr<TreeNode>> m_fields;

        std::vector<std::string> m_inheritances, m_cppInheritances;

        std::string m_name;
    };
}