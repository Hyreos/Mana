#include "ExportQualifier.hh"

namespace mana::ast {
    // Export Qualifier
    ExportQualifier::ExportQualifier() 
    {
    }

    const ExportQualifier* ExportQualifier::clone(CloneContext& ctx) const
    {
        return ctx.create<ExportQualifier>();
    }

    void ExportQualifier::print(std::ostream& stream, size_t ident) const
    {
        stream << "export";
    }
}

MANA_RTTI_TYPE(mana::ast::ExportQualifier);