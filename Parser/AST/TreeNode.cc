#include "TreeNode.hh"

namespace mana {
    TreeNode::TreeNode(ASTKind kind) 
        : m_kind{ kind } 
    {
    }

    void TreeNode::addAttribute(std::unique_ptr<TreeNode> attr)
    {
        m_attributes.push_back(attr->clone());
    }

    std::vector<std::unique_ptr<TreeNode>>& TreeNode::attributes()
    {
        return m_attributes;
    }
}