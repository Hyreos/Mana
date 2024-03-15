#pragma once

#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>
#include <absl/types/variant.h>

namespace mana::ast {
    class BinaryOp : public TreeNode {
    public:
        struct TypeMask {
            enum Enum {
                kSum = 0x1,
                kDiv = 0x2,
                kMod = 0x4,
                kMul = 0x8,
                kSub = 0x10,
                kComma = 0x20,
                kScopeRes = 0x40
            };
        };

        static constexpr TreeNode::Type baseType = TreeNode::Type::kBinaryOp;

        BinaryOp(
            TypeMask::Enum type,
            std::unique_ptr<TreeNode> lhs,
            std::unique_ptr<TreeNode> rhs
        );

        TreeNode* lhs();

        TreeNode* rhs();

        TypeMask::Enum type() const;
    protected:
        TypeMask::Enum m_type;
        
        std::unique_ptr<TreeNode> m_lhs, m_rhs;
    };
}