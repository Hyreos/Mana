#include "BinaryExpression.hh"

namespace mana::ast {
    BinaryExpression::BinaryExpression(
        const Expression* lhs, 
        OpType type,
        const Expression* rhs
    ) : m_lhs{ lhs },
        m_rhs{ rhs },
        m_type { type }
    {
    }

    const Expression* BinaryExpression::lhs() 
    {
        return m_lhs;
    }

    const Expression* BinaryExpression::rhs()
    {
        return m_rhs;
    }

    BinaryExpression::OpType BinaryExpression::type() const
    {
        return m_type;
    }

    void BinaryExpression::print(std::ostream& stream, size_t ident) const
    {
        stream << " ( ";

        m_lhs->print(stream, ident);

        switch (m_type) {
            case OpType::kAdd:
                stream << " + ";
                break;
            case OpType::kComma:
                stream << ", ";
                break;
            case OpType::kDivide:
                stream << " / ";
                break;
            case OpType::kModulus:
                stream << " % ";
                break;
            case OpType::kMultiply:
                stream << " * ";
                break;
            case OpType::kScopeResolution:
                stream << " :: ";
                break;
            case OpType::kSubtract:
                stream << " - ";
                break;
            default:
                stream << " ?? ";
        }

        m_rhs->print(stream, ident);

        stream << " ) ";
    }

    const BinaryExpression* BinaryExpression::clone(CloneContext& ctx) const
    {
        return ctx.create<BinaryExpression>(
            ctx.clone(m_lhs),
            m_type,
            ctx.clone(m_rhs)
        );
    }
}