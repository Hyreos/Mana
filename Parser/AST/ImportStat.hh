#pragma once

#include "TreeNode.hh"

#include <filesystem>

namespace mana {
    class ImportStat : public TreeNode {
    public:
        static constexpr TreeNode::Type kind { TreeNode::Type::kImportStat };

        ImportStat(
            std::vector<std::filesystem::path> pathlist
        );

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;
    private:
        std::vector<std::filesystem::path> m_pathlist;
    };
}