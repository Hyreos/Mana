#include "TreeNode.hh"

namespace mana::ast {
    TreeNode::TreeNode(TreeNode::Type baseType) 
        : m_kind{ baseType } 
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