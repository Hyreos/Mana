#pragma once

#include "Utils/RTTI.hh"

#include "Declaration.hh"
#include "Attribute.hh"

#include <filesystem>

namespace mana::ast {
    class ImportDeclaration final : public rtti::Castable<ImportDeclaration, Declaration> {
    public:
        ImportDeclaration(
            const std::vector<std::filesystem::path>& pathlist,
            bool is_cc,
            const std::vector<const Attribute*>& attributes
        );

        const ImportDeclaration* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;
    private:
        const std::vector<std::filesystem::path> m_pathlist;
        const std::vector<const Attribute*> m_attributes;
        bool m_isCc;
    };
}

MANA_RTTI_TYPE(mana::ast::ImportDeclaration);