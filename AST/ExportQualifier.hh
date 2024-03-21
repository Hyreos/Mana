#pragma once

#include "Qualifier.hh"

#include "Declaration.hh"
#include "Utils/RTTI.hh"

#include <string>
#include <variant>

namespace mana::ast {
    class ExportQualifier final : public rtti::Castable<ExportQualifier, Qualifier> {
    public:
        ExportQualifier();

        const ExportQualifier* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;
    };
}
