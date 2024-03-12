#include "Component.hh"

namespace mana {
    Component::Component(
        const std::string& name,
        std::vector<std::unique_ptr<TreeNode>> fields
    ) 
        : TreeNode(kind),
            m_fields{ std::move(fields) },
            m_name{ name }
    {
    }

    std::unique_ptr<TreeNode> Component::clone()
    {
        std::vector<std::unique_ptr<TreeNode>> cfields;

        for (auto& f : m_fields) cfields.push_back(f->clone());

        return std::make_unique<Component>(m_name, std::move(cfields));
    }

    void Component::print(std::ostream& stream, size_t ident)
    {
        stream << "component " << m_name << " {" << std::endl;

        for (auto& f : m_fields) {
            std::cout << std::string(2 * ident + 2, ' ');
            
            f->print(stream, ident + 1);

            stream << std::endl;
        }
        
        stream << "}";
    }
}