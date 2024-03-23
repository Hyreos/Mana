#include "Component.hh"

namespace mana::sem {
    ast::ComponentDeclaration* Component::ast()
    {
        return m_ast;
    }
}