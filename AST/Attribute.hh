#pragma once

#include "TreeNode.hh"
#include "Utils/RTTI.hh"

#include <string>

namespace mana::ast {
    class Attribute : public rtti::Castable<Attribute, TreeNode> {
    public:
        virtual std::string name() const = 0;
    };
}