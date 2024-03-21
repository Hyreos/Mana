#pragma once

#include "Utils/RTTI.hh"
#include "IdentifierExpression.hh"
#include "TreeNode.hh"

namespace mana::ast {
    class Type final : public rtti::Castable<Type, TreeNode> {
    public:
        Type(const IdentifierExpression* type_identifier, const Type* subtype = nullptr);

        void print(std::ostream& stream, size_t ident) const override;

        const Type* clone(CloneContext& ctx) const override;

        const IdentifierExpression* symbol() const;
    private:
        const IdentifierExpression* m_symbol;
        const Type* m_subtype;
    };
}

