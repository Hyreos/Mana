#include "ReadonlyQualifier.hh"

namespace mana::ast {
    // Readonly Qualifier
    ReadonlyQualifier::ReadonlyQualifier(const Declaration* declaration) 
        : m_declaration { declaration }
    {
    }

    const ReadonlyQualifier* ReadonlyQualifier::clone(CloneContext& ctx) const
    {
        return ctx.create<ReadonlyQualifier>(ctx.clone(m_declaration));
    }

    void ReadonlyQualifier::print(std::ostream& stream, size_t ident) const
    {
        stream << "readonly ";

        m_declaration->print(stream, ident);
    }
}