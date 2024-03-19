#include "ReadonlyQualifier.hh"

namespace mana::ast {
    // Readonly Qualifier
    ReadonlyQualifier::ReadonlyQualifier() 
    {
    }

    const ReadonlyQualifier* ReadonlyQualifier::clone(CloneContext& ctx) const
    {
        return ctx.create<ReadonlyQualifier>();
    }

    void ReadonlyQualifier::print(std::ostream& stream, size_t ident) const
    {
        stream << "readonly";
    }
}