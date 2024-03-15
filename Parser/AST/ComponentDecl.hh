#pragma once

#include "TreeNode.hh"

#include "Declaration.hh"

namespace mana::ast {
    class ComponentDecl : public  Declaration {
    public:
        ComponentDecl(
            const std::string& name,
            std::vector<std::unique_ptr<TreeNode>> fields,
            std::vector<std::unique_ptr<TreeNode>> inheritances,
            bool isExported
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