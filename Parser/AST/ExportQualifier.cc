#include "ExportQualifier.hh"

namespace mana::ast {
    // Export Qualifier
    ExportQualifier::ExportQualifier(const Declaration* declaration) 
        : m_declaration { declaration }
    {
    }

    const ExportQualifier* ExportQualifier::clone(CloneContext& ctx) const
    {
        return ctx.create<ExportQualifier>(ctx.clone(m_declaration));
    }

    void ExportQualifier::print(std::ostream& stream, size_t ident) const
    {
        stream << "export ";

        m_declaration->print(stream, ident);
    }
}