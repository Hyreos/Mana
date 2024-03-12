#include "Number.hh"

namespace mana {
    Number::Number(std::variant<double, int64_t> value) 
        : TreeNode(kind),
            m_value { value }
    {
    }

    std::unique_ptr<TreeNode> Number::clone()
    {
        return std::make_unique<Number>(m_value);
    }

    void Number::print(std::ostream& stream, size_t ident)
    {
        std::visit([&](auto&& arg) {
            stream << arg;
        }, m_value);
    }
}