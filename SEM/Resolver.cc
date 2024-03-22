#include "Resolver.hh"

#include "AST/BinaryExpression.hh"
#include "AST/IntegerLiteralExpression.hh"

namespace mana {
    Resolver::Resolver(const sem::Module* module)
    {
    }

    const sem::Module* Resolver::root() const {
        return m_module;
    }

    std::unique_ptr<sem::Expression> Resolver::expression(const ast::Expression* expression) {
        if (expression->is<ast::BinaryExpression>()) {
            auto expr = expression->as<ast::BinaryExpression>();

            auto lhs = expr->lhs();
            auto rhs = expr->rhs();

            if (lhs->is<ast::LiteralExpression>() && rhs->is<ast::LiteralExpression>()) {
                0
            } else {

            }
        } else if (expression->is<ast::LiteralExpression>()) {
            return std::make_unique<sem::Expression>(expression);
        } else {
            // TODO: throw error
            std::cerr << "TODO ERROR" << std::endl;
            return nullptr;
        }
    }
}
