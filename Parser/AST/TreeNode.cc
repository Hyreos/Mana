#include "TreeNode.hh"

#include "Attribute.hh"

namespace mana::ast {
    TreeNode::TreeNode()  
    {
    }

    void TreeNode::print(std::ostream& stream, size_t ident) const {
        for (auto& attr : m_attributes) attr->print(stream, ident);

        if (!m_attributes.empty())
            stream << " ";
    }

    void TreeNode::addAttribute(const Attribute* attr)
    {
        m_attributes.push_back(attr);
    }

    std::vector<const Attribute*>& TreeNode::attributes()
    {
        return m_attributes;
    }
}