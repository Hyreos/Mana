#pragma once

#include "Utils/RTTI.hh"

#include "Declaration.hh"

#include <filesystem>

namespace mana::ast {
    class ImportDeclaration final : public rtti::Castable<ImportDeclaration, Declaration> {
    public:
        ImportDeclaration(
            std::vector<std::filesystem::path> pathlist,
            bool is_cc = false
        );

        const ImportDeclaration* clone(CloneContext& ctx) const override;

        void print(std::ostream& stream, size_t ident) const override;
    private:
        std::vector<std::filesystem::path> m_pathlist;

        bool m_isCc;
    };
}

MANA_RTTI_TYPE(mana::ast::ImportDeclaration);