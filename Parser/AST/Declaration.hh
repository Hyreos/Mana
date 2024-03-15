#pragma once

#include "TreeNode.hh"

namespace mana::ast {
    class Declaration : public TreeNode {
    public:
        static constexpr Type baseType = Type::kDeclaration;

        enum class Kind {
            kComponent,
            kImport,
            kMember
        };

        Declaration(Kind kind);

        Kind kind() const;
    private:
        Kind m_kind;
    };
}