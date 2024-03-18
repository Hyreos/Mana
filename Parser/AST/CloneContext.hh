#pragma once

#include <vector>

namespace mana::ast {
    class TreeNode;

    class CloneContext {
    public:
        template<typename T, typename... Args>
        const T* create(Args&&... args) 
        {
            auto ptr = new T(std::forward<Args>(args)...);
            m_nodes.push_back(static_cast<TreeNode*>(ptr));
            return ptr;
        }

        template<typename T>
        const T* clone(const T* obj) {
            if (auto cloned = cloneNode(obj)) return static_cast<const T*>(cloned);
            else return nullptr;
        }

        template<typename T>
        const std::vector<const T*> clone(const std::vector<const T*>& array)
        {
            std::vector<const T*> c;

            for (const T* node : array)
                c.push_back(clone(node));

            return c;
        } 

        template<typename T>
        const T* cloneNode(const T* obj) {
            return static_cast<const T*>(obj->clone(*this));
        }

        inline void destroyAll()
        {
            for (auto& node : m_nodes) delete node;
            
            m_nodes.clear();
        }
    private:
        std::vector<TreeNode*> m_nodes;
    };
}