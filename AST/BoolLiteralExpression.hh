#include "LiteralExpression.hh"

namespace mana::ast {
    class BoolLiteralExpression final : public rtti::Castable<BoolLiteralExpression, LiteralExpression> {
    public:
        BoolLiteralExpression(bool value);
    
        const BoolLiteralExpression* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;
    private:
        bool m_value;
    };
}

MANA_RTTI_TYPE(mana::ast::BoolLiteralExpression);
