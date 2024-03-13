#include "Attribute.hh"

namespace mana {
    Attribute::Attribute(
        const std::string& name,
        std::unique_ptr<TreeNode> value
    ) 
        : TreeNode(kind),
            m_name { name },
            m_value { std::move(value) } 
    {
    }

    std::unique_ptr<TreeNode> Attribute::clone()
    {
        return std::make_unique<Attribute>(m_name, (m_value) ? m_value->clone() : nullptr);
    }

    void Attribute::print(std::ostream& stream, size_t ident)
    {
        stream << "@" << m_name;
        
        if (m_value) {
            stream << "(";
            m_value->pprint(stream, ident);
            stream << ")";
        } 
    }
}