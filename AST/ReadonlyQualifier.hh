#pragma once

#include "Qualifier.hh"

#include "Declaration.hh"
#include "Utils/RTTI.hh"

#include <string>
#include <variant>

namespace mana::ast {
    class ReadonlyQualifier final : public rtti::Castable<ReadonlyQualifier, Qualifier> {
    public:
        ReadonlyQualifier();

        const ReadonlyQualifier* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;
    };
}

