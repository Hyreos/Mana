#include "Qualifier.hh"

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
        TreeNode::print(stream, ident);

        stream << "readonly ";

        m_declaration->print(stream, ident);
    }

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
        TreeNode::print(stream, ident);

        stream << "export ";

        m_declaration->print(stream, ident);
    }
}