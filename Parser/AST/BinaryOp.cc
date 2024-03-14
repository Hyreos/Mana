#include "BinaryOp.hh"

#include "TreeTransverser.hh"

#include "Helpers/Macros.hh"

namespace mana {
    BinaryOp::BinaryOp(
        TypeMask::Enum type,
        std::unique_ptr<TreeNode> lhs, 
        std::unique_ptr<TreeNode> rhs
    )
        : TreeNode(kind),
            m_lhs{ std::move(lhs) },
            m_rhs{ std::move(rhs) },
            m_type{ type }
    {
    }

    TreeNode* BinaryOp::lhs() 
    {
        return m_lhs.get();
    }

    TreeNode* BinaryOp::rhs()
    {
        return m_rhs.get();
    }

    BinaryOp::TypeMask::Enum BinaryOp::type() const
    {
        return m_type;
    }
}