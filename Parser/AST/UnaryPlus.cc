#include "UnaryPlus.hh"

namespace mana {
    UnaryPlus::UnaryPlus(std::unique_ptr<TreeNode> operand)
        : TreeNode(kind),
            m_operand{std::move(operand)}
    {
    }

    std::unique_ptr<TreeNode> UnaryPlus::clone()
    {
        return std::make_unique<UnaryPlus>(m_operand->clone());
    }

    void UnaryPlus::print(std::ostream& stream, size_t ident)
    {
        stream << "(";

        stream << "+";

        m_operand->print(stream, ident);

        stream << ")";
    }
}