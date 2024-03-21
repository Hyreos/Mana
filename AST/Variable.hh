#pragma once

#include "TreeNode.hh"

namespace mana::ast {
    class Variable : public rtti::Castable<Variable, TreeNode> {};
}