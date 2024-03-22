#pragma once

#include "TreeNode.hh"

#include "Utils/RTTI.hh"

namespace mana::ast {
    class Declaration : public rtti::Castable<Declaration, TreeNode> 
    {
    };
}
