#pragma once

#include "Declaration.hh"

#include <optional>
#include <string>

namespace mana::ast {
    class MemberDecl : public Declaration {
    public:
        MemberDecl(
            std::unique_ptr<TreeNode> type,
            const std::string& name,
            std::unique_ptr<TreeNode> defaultValue,
            bool isOptional,
            std::optional<std::string> cppPropName = std::nullopt
        );

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        const std::string& name() const;

        void accept(TreeVisitor* visitor) override;
    private:
        std::string m_name;
        bool m_isOptional;
        std::unique_ptr<TreeNode> m_type, m_defaultValue;
        std::optional<std::string> m_cppPropName;
    };
}