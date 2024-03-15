#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>

#include "Literal.hh"

namespace mana::ast {
    class StrLit : public Literal {
    public:
        StrLit(const std::string& str);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void accept(TreeVisitor* visitor) override;
    private:
        std::string m_value;
    };
}