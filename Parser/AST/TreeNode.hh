#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "Helpers/Deleted.hh"

namespace mana::ast {
    class TreeVisitor;

    class TreeNode {
    public:
        enum class Type {
            kAttribute,
            kTSymbol,
            kDeclaration,
            kUnary,

            kQualifier,

            kType,
            kBinaryOp,

            kUnaryMinus,
            kUnaryPlus,

            kLiteral,

            kExpr
        };

        TreeNode(TreeNode::Type baseType);

        virtual ~TreeNode() = default;

        virtual std::unique_ptr<TreeNode> clone() = 0;
        
        template<typename T>
        T* cast() 
        {
            if (m_kind != T::baseType) return nullptr;

            return static_cast<T*>(this);
        }

        virtual void print(std::ostream& stream, size_t ident) {
            for (auto& attr : m_attributes) attr->print(stream, ident);

            if (!m_attributes.empty())
                stream << " ";
        }

        void addAttribute(std::unique_ptr<TreeNode> attr);

        std::vector<std::unique_ptr<TreeNode>>& attributes();

        virtual void accept(TreeVisitor* visitor) = 0;
    private:
        TreeNode::Type m_kind;

        std::vector<std::unique_ptr<TreeNode>> m_attributes;
    };
}