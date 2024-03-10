#include "Component.hh"

namespace mona {
    Component::Component(
        std::vector<DeletedUnique_T<TreeNode>> fields
    ) 
        : TreeNode(kind),
            m_fields{ std::move(fields) }
    {
    }

    DeletedUnique_T<TreeNode> Component::clone()
    {
        std::vector<DeletedUnique_T<TreeNode>> cfields;

        for (auto& f : m_fields) cfields.push_back(f->clone());

        return MakeUniquePtr<Component>(std::move(cfields));
    }
}