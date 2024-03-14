#include "CompFieldDecl.hh"

#include "TreeTransverser.hh"

namespace mana {
    CompFieldDecl::CompFieldDecl(
        std::unique_ptr<TreeNode> type,
        const std::string& name,
        std::unique_ptr<TreeNode> defaultValue,
        bool isOptional,
        std::optional<std::string> propCppName
    ) 
        : TreeNode(kind),
            m_name{ name },
            m_type{ std::move(type) },
            m_defaultValue { std::move(defaultValue) },
            m_cppPropName { propCppName },
            m_isOptional { isOptional }
    {
    }

    std::unique_ptr<TreeNode> CompFieldDecl::clone()
    {
        return std::make_unique<CompFieldDecl>(
            m_type->clone(), 
            m_name, 
            m_defaultValue->clone(), 
            m_isOptional, 
            m_cppPropName
        );
    }

    void CompFieldDecl::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        for (auto& attr : attributes())
            attr->print(stream, ident);

        m_type->print(stream, ident);

        if (m_isOptional) stream << "?";

        stream << " ";

        stream << m_name;

        if (m_cppPropName)
            stream << " -> " << *m_cppPropName;

        if (m_defaultValue) {
            stream << " = ";

            m_defaultValue->print(stream, ident);
        }
    }

    const std::string& CompFieldDecl::name() const
    {
        return m_name;
    }

    void CompFieldDecl::accept(TreeVisitor* visitor)
    {
        visitor->visitDeclaration(this);
    };
}