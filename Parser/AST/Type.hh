#pragma once

#include "TreeNode.hh"

#include <string>

namespace mana::ast {
    class Type : public TreeNode {
    public:
        static constexpr TreeNode::Type baseType { TreeNode::Type::kType };

        enum class Kind {
            kI16,
            kI32,
            kI64,
            kU16,
            kU32,
            kU64,
            kStr
        };

        Type(Kind kind);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void accept(TreeVisitor* visitor) override;
    private:
        Kind m_kind;
    };
}