#pragma once

#include "TreeNode.hh"

namespace mana::ast {
    class Unary : public TreeNode {
    public:
        enum class Kind {
            kMinus, kPlus
        };

        Unary(Kind kind);
    private:
        Kind m_kind;
    };
}