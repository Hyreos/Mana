#include "TreeNode.hh"

namespace mona {
    TreeNode::TreeNode(ASTKind kind) 
        : m_kind{ kind } 
    {
    }

    void TreeNode::addAttribute(DeletedUnique_T<TreeNode> attr)
    {
        m_attributes.push_back(attr->clone());
    }

    std::vector<DeletedUnique_T<TreeNode>>& TreeNode::attributes()
    {
        return m_attributes;
    }
}