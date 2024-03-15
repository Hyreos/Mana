#include "MemberDecl.hh"

#include "TreeTransverser.hh"

namespace mana::ast {
    MemberDecl::MemberDecl(
        std::unique_ptr<TreeNode> type,
        const std::string& name,
        std::unique_ptr<TreeNode> defaultValue,
        bool isOptional,
        std::optional<std::string> propCppName
    ) 
        : Declaration(Declaration::Kind::kMember),
            m_name{ name },
            m_type{ std::move(type) },
            m_defaultValue { std::move(defaultValue) },
            m_cppPropName { propCppName },
            m_isOptional { isOptional }
    {
    }

    std::unique_ptr<TreeNode> MemberDecl::clone()
    {
        return std::make_unique<MemberDecl>(
            m_type->clone(), 
            m_name, 
            m_defaultValue->clone(), 
            m_isOptional, 
            m_cppPropName
        );
    }

    void MemberDecl::print(std::ostream& stream, size_t ident)
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

    const std::string& MemberDecl::name() const
    {
        return m_name;
    }

    void MemberDecl::accept(TreeVisitor* visitor)
    {
        visitor->visit(this);
    };
}