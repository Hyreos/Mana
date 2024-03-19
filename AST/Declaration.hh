#pragma once

#include "TreeNode.hh"

#include "Utils/RTTI.hh"

namespace mana::ast {
    class Declaration : public rtti::Castable<Declaration, TreeNode> 
    { 
    };
}

MANA_RTTI_TYPE(mana::ast::Declaration)