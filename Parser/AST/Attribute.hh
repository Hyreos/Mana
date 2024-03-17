#pragma once

#include "TreeNode.hh"
#include "Utils/RTTI.hh"

#include <string>

namespace mana::ast {
    class Attribute : public rtti::Castable<Attribute, TreeNode> {
    public:
        Attribute(const std::string& name);

        const std::string& name() const;
    private:
        std::string m_name;
    };
}

MANA_RTTI_TYPE(mana::ast::Attribute)