#include "ImportDeclaration.hh"

namespace mana::ast {
    ImportDeclaration::ImportDeclaration(
        std::vector<std::filesystem::path> pathlist,
        bool iscc
    ) 
        : m_pathlist{ std::move(pathlist) },
            m_isCc { iscc }
    {
    }

    const ImportDeclaration* ImportDeclaration::clone(CloneContext& ctx) const
    {
        return ctx.create<ImportDeclaration>(m_pathlist, m_isCc);
    }

    void ImportDeclaration::print(std::ostream& stream, size_t ident) const
    {
        TreeNode::print(stream, ident);

        stream << "import ("<< std::endl;

        for (auto i = 0; i < m_pathlist.size(); i++) {
            stream << std::string(2 * ident + 2, ' ') << m_pathlist[i] << std::endl;
        }

        stream << ")" << std::endl;
    }
}