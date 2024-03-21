#pragma once

#include "TreeNode.hh"
#include "Declaration.hh"
#include "Utils/RTTI.hh"

#include <string>
#include <variant>

namespace mana::ast {
    struct Qualifier : public rtti::Castable<Qualifier, TreeNode> 
    {
    };
}

