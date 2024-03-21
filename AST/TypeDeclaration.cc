#include "TypeDeclaration.hh"

namespace mana::ast {
    const std::string& TypeDeclaration::name() const
    {
        return m_name;
    }
}

MANA_RTTI_TYPE(mana::ast::TypeDeclaration);