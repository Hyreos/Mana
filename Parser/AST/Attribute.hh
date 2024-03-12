#pragma once

#include "TreeNode.hh"

#include <string>

namespace mona {
    class Attribute : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kAttribute };

        Attribute(
            const std::string& name,
            std::unique_ptr<TreeNode> value
        );

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        std::string m_name;
        std::unique_ptr<TreeNode> m_value;
    };
}