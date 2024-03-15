#include "Declaration.hh"

namespace mana::ast {
    Declaration::Declaration(Declaration::Kind _kind) 
        : TreeNode { TreeNode::Type::kDeclaration },
            m_kind { _kind }
    {
    }

    Declaration::Kind Declaration::kind() const
    {
        return m_kind;
    }
}