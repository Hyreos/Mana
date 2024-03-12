#pragma once

#include "TreeNode.hh"

#include <string>

namespace mona {
    class Number : public TreeNode {
    public:
        static constexpr ASTKind kind { ASTKind::kNumber };

        Number(const std::string& identifier);

        DeletedUnique_T<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        std::string m_identifier;
    };
}