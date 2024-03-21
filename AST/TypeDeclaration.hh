#pragma once

#include "Declaration.hh"

#include "Utils/RTTI.hh"

namespace mana::ast {
    class TypeDeclaration : public rtti::Castable<TypeDeclaration, Declaration> {
    public:
        const std::string& name() const;

        std::string m_name;
    };
}

