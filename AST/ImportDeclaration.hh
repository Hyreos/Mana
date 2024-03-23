#pragma once

#include "Utils/RTTI.hh"

#include "Declaration.hh"
#include "Attribute.hh"
#include "IdentifierExpression.hh"

#include <filesystem>

namespace mana::ast {
    class ImportDeclaration final : public rtti::Castable<ImportDeclaration, Declaration> {
    public:
        ImportDeclaration(
            const ast::IdentifierExpression* module_identifier,
            const std::filesystem::path& path,
            const std::vector<const Attribute*>& attributes
        );

        const ImportDeclaration* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;

        const std::filesystem::path& path() const;
    private:
        const std::filesystem::path m_path;
        const std::vector<const Attribute*> m_attributes;
        const ast::IdentifierExpression* m_module_identifier;
    };
}
