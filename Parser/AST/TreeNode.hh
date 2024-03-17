#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "Helpers/Deleted.hh"
#include "CloneContext.hh"

#include "Utils/RTTI.hh"

namespace mana::ast {
    class TreeVisitor;

    class Attribute;

    class TreeNode : public rtti::Castable<TreeNode, rtti::Base> {
    public:
        TreeNode();

        virtual ~TreeNode() = default;

        virtual const TreeNode* clone(CloneContext& ctx) const = 0;
        
        virtual void print(std::ostream& stream, size_t ident) const;

        void addAttribute(const Attribute* attr);

        std::vector<const Attribute*>& attributes();
    private:
        std::vector<const Attribute*> m_attributes;
    };
}

MANA_RTTI_TYPE(mana::ast::TreeNode);