#include "MemberDeclaration.hh"

namespace mana::ast {
    MemberDeclaration::MemberDeclaration(
        const Type* type,
        const std::string& name,
        const Expression* defaultValue,
        bool isOptional,
        const std::vector<const Attribute*> attributes,
        const std::vector<const Qualifier*> qualifiers
    ) 
        : m_name{ name },
            m_type{ std::move(type) },
            m_default { std::move(defaultValue) },
            m_optional { isOptional },
            m_attributes { attributes },
            m_qualifiers { qualifiers }
    {
    }

    const MemberDeclaration* MemberDeclaration::clone(CloneContext& ctx) const
    {
        return ctx.create<MemberDeclaration>(
            ctx.clone(m_type),
            m_name,
            ctx.clone(m_default),
            m_optional,
            ctx.clone(m_attributes),
            ctx.clone(m_qualifiers)
        );
    }

    const Type* MemberDeclaration::type() const
    {
        return m_type;
    }

    void MemberDeclaration::print(std::ostream& stream, size_t ident) const
    {
        for (auto& qualifier : m_qualifiers) {
            qualifier->print(stream, ident);
            stream << " ";
        }

        for (auto& attribute : m_attributes) {
            attribute->print(stream, ident);
            stream << " ";
        }

        m_type->print(stream, ident);

        if (m_optional) 
            stream << "?";

        stream << " ";

        stream << m_name;

        if (m_default) {
            stream << " = ";

            m_default->print(stream, ident);
        }
    }

    const std::string& MemberDeclaration::name() const
    {
        return m_name;
    }
}

MANA_RTTI_TYPE(mana::ast::MemberDeclaration);