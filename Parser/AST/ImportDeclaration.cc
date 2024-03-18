#include "ImportDeclaration.hh"

namespace mana::ast {
    ImportDeclaration::ImportDeclaration(
        const std::vector<std::filesystem::path>& pathlist,
        bool iscc,
        const std::vector<const Attribute*>& attributes
    ) 
        : m_pathlist{ std::move(pathlist) },
            m_isCc { iscc },
            m_attributes { attributes }
    {
    }

    const ImportDeclaration* ImportDeclaration::clone(CloneContext& ctx) const
    {
        return ctx.create<ImportDeclaration>(m_pathlist, m_isCc, ctx.clone(m_attributes));
    }

    void ImportDeclaration::print(std::ostream& stream, size_t ident) const
    {
        for (auto* attr : m_attributes) {
            attr->print(stream, ident);

            stream << " ";
        }

        stream << "import ("<< std::endl;

        for (auto i = 0; i < m_pathlist.size(); i++) {
            stream << std::string(2 * ident + 2, ' ') << m_pathlist[i] << std::endl;
        }

        stream << ")" << std::endl;
    }
}