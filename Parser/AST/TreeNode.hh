#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "Helpers/Deleted.hh"

namespace mana {
    class TreeVisitor;

    class TreeNode {
    public:
        enum class Type {
            kAttribute,
            kComponent,
            kComponentField,
            kTSymbol,

            kType,
            kBinaryOp,

            kScopeResolution,

            kImportStat,

            kUnaryMinus,
            kUnaryPlus,

            kLiteral,

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