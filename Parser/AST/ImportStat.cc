#include "ImportStat.hh"

namespace mana {
    ImportStat::ImportStat(
        std::vector<std::filesystem::path> pathlist
    ) 
        : TreeNode(kind),
            m_pathlist{ std::move(pathlist) }
    {
    }

    std::unique_ptr<TreeNode> ImportStat::clone()
    {
        return std::make_unique<ImportStat>(m_pathlist);
    }

    void ImportStat::print(std::ostream& stream, size_t ident)
    {
        stream << "import (" << std::endl;

        for (auto i = 0; i < m_pathlist.size(); i++) {
            stream << std::string(2 * ident + 2, ' ') << m_pathlist[i] << std::endl;
        }

        stream << ")" << std::endl;
    }
}