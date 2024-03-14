#include "ImportDecl.hh"

#include "TreeTransverser.hh"

namespace mana {
    ImportDecl::ImportDecl(
        std::vector<std::filesystem::path> pathlist,
        bool iscc
    ) 
        : TreeNode(kind),
            m_pathlist{ std::move(pathlist) },
            m_isCc { iscc }
    {
    }

    std::unique_ptr<TreeNode> ImportDecl::clone()
    {
        return std::make_unique<ImportDecl>(m_pathlist, m_isCc);
    }

    void ImportDecl::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "import ("<< std::endl;

        for (auto i = 0; i < m_pathlist.size(); i++) {
            stream << std::string(2 * ident + 2, ' ') << m_pathlist[i] << std::endl;
        }

        stream << ")" << std::endl;
    }

    void ImportDecl::accept(TreeVisitor* visitor)
    {
        visitor->visitStatement(this);
    }
}