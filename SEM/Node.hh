#pragma once

#include "Utils/RTTI.hh"

namespace mana::sem {
    class Node : public rtti::Castable<Node, rtti::Base>
    {
    };
}