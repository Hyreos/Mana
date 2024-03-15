#include "Unary.hh"

namespace mana::ast {
    Unary::Unary(Kind kind) : 
        TreeNode { TreeNode::Type::kUnary },
        m_kind{ kind }
    {
    }
}