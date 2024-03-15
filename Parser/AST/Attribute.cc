#include "Attribute.hh"

#include "TreeTransverser.hh"

namespace mana::ast {
    Attribute::Attribute(
        const std::string& name,
        std::unique_ptr<TreeNode> value
    ) 
        : TreeNode(baseType),
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
        TreeNode::print(stream, ident);

        stream << "@" << m_name;
        
        if (m_value) {
            stream << "(";
            m_value->print(stream, ident);
            stream << ")";
        } 
    }

    void Attribute::accept(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }
}