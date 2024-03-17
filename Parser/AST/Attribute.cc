#include "Attribute.hh"

namespace mana::ast {
    Attribute::Attribute(const std::string& name) 
        : m_name { name }
    {
    }

    const std::string& Attribute::name() const
    {
        return m_name;
    }
}