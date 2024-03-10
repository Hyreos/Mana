#pragma once

#include <vector>
#include <memory>

#include "Helpers/Deleted.hh"

namespace mona {
    enum class ASTKind {
        kAttribute,
        kComponent,
        kComponentField,
        kTSymbol,
        kNumber
    };

    class TreeNode {
    public:
        TreeNode(ASTKind kind);

        virtual DeletedUnique_T<TreeNode> clone() = 0;

        template<typename T>
        T* cast() 
        {
            if (m_kind != T::kind) return nullptr;

            return static_cast<T*>(this);
        }

        void addAttribute(DeletedUnique_T<TreeNode> attr);

        std::vector<DeletedUnique_T<TreeNode>>& attributes();
    private:
        ASTKind m_kind;

        std::vector<DeletedUnique_T<TreeNode>> m_attributes;
    };
}