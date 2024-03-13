#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "Helpers/Deleted.hh"

namespace mana {
    class TreeNode {
    public:
        enum class Type {
            kAttribute,
            kComponent,
            kComponentField,
            kTSymbol,
            kAdd,
            kSub,
            kSlash,
            kMul,
            kMod,

            kImportStat,

            kUnaryMinus,
            kUnaryPlus,

            kInt64Lit,
            kInt32Lit,
            kInt16Lit,

            kUint64Lit,
            kUint32Lit,
            kUint16Lit,

            kExpr
        };

        TreeNode(TreeNode::Type kind);

        virtual ~TreeNode() = default;

        virtual std::unique_ptr<TreeNode> clone() = 0;
        
        template<typename T>
        T* cast() 
        {
            if (m_kind != T::kind) return nullptr;

            return static_cast<T*>(this);
        }

        inline TreeNode::Type kind() const
        {
            return m_kind;
        }

        virtual void print(std::ostream& stream, size_t ident) = 0;

        void addAttribute(std::unique_ptr<TreeNode> attr);

        std::vector<std::unique_ptr<TreeNode>>& attributes();
    private:
        TreeNode::Type m_kind;

        std::vector<std::unique_ptr<TreeNode>> m_attributes;
    };
}