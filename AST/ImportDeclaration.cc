#include "ImportDeclaration.hh"

namespace mana::ast {
    ImportDeclaration::ImportDeclaration(
        const ast::IdentifierExpression* module_identifier,
        const std::filesystem::path& path,
        const std::vector<const Attribute*>& attributes
    ) 
        : m_path { std::move(path) },
            m_attributes { attributes },
            m_module_identifier { module_identifier }
    {
    }

    const ImportDeclaration* ImportDeclaration::clone(CloneContext& ctx) const
    {
        return ctx.create<ImportDeclaration>(m_module_identifier, m_path, ctx.clone(m_attributes));
    }

    void ImportDeclaration::print(std::ostream& stream, size_t ident) const
    {
        for (auto* attr : m_attributes) {
            attr->print(stream, ident);

            stream << " ";
        }

        stream << "import ";

        m_module_identifier->print(stream, ident);

        stream << " from '" << m_path << "'" << std::endl;
    }

    const std::filesystem::path& ImportDeclaration::path() const
    {
        return m_path;
    }
}

MANA_RTTI_TYPE(mana::ast::ImportDeclaration);