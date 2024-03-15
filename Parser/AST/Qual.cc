#include "Qual.hh"

#include "TreeTransverser.hh"

#include "Helpers/Macros.hh"

namespace mana::ast {
    Qualifier::Qualifier(Kind _kind, std::unique_ptr<TreeNode> node) :
        TreeNode { TreeNode::Type::kQualifier },
        m_kind { _kind },
        m_node { std::move(node) }
    {
    }

    Qualifier::Kind Qualifier::kind() const
    {
        return m_kind;
    }

    // Readonly Qualifier
    ReadonlyQualifier::ReadonlyQualifier(std::unique_ptr<TreeNode> node) 
        : Qualifier(Qualifier::Kind::kReadonlyQualifier, std::move(node))
    {
    }

    std::unique_ptr<TreeNode> ReadonlyQualifier::clone()
    {
        return std::make_unique<ReadonlyQualifier>(m_node->clone());
    }

    void ReadonlyQualifier::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "readonly ";

        m_node->print(stream, ident);
    }

    void ReadonlyQualifier::accept(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }

    // Export Qualifier
    ExportQualifier::ExportQualifier(std::unique_ptr<TreeNode> node) 
        : Qualifier(Qualifier::Kind::kExportQualifier, std::move(node))
    {
    }

    std::unique_ptr<TreeNode> ExportQualifier::clone()
    {
        return std::make_unique<ExportQualifier>(m_node->clone());
    }

    void ExportQualifier::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        stream << "export ";

        m_node->print(stream, ident);
    }

    void ExportQualifier::accept(TreeVisitor* visitor)
    {
        visitor->visit(this);
    }
}