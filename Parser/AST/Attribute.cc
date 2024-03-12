#include "Attribute.hh"

namespace mona {
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
        return std::make_unique<Attribute>(m_name, m_value->clone());
    }

    void Attribute::print(std::ostream& stream, size_t ident)
    {
        stream << "@" << m_name << "(";
        m_value->print(stream, ident);
        stream << ")"; 
    }
}