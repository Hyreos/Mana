#include "Module.hh"

namespace mana::sem {
    ast::Module* Module::ast()
    {
        return m_module;
    }
}

MANA_RTTI_TYPE(mana::sem::Module)