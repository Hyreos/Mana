#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "Helpers/Deleted.hh"

namespace mana {
    enum class ASTKind {
        kAttribute,
        kComponent,
        kComponentField,
        kTSymbol,
        kAdd,
        kSub,
        kSlash,
        kMul,
        kMod,

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

    class TreeNode {
    public:
        TreeNode(ASTKind kind);

        virtual ~TreeNode() = default;

        virtual std::unique_ptr<TreeNode> clone() = 0;
        
        template<typename T>
        T* cast() 
        {
            if (m_kind != T::kind) return nullptr;

            return static_cast<T*>(this);
        }

        inline ASTKind kind() const
        {
            return m_kind;
        }

        virtual void print(std::ostream& stream, size_t ident) = 0;

        void addAttribute(std::unique_ptr<TreeNode> attr);

        std::vector<std::unique_ptr<TreeNode>>& attributes();
    private:
        ASTKind m_kind;

        std::vector<std::unique_ptr<TreeNode>> m_attributes;
    };
}