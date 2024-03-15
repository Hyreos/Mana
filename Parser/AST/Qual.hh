#pragma once

#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>
#include <absl/types/variant.h>

namespace mana::ast {
    class Qualifier : public TreeNode {
    public:
        static constexpr Type baseType = Type::kQualifier;

        enum class Kind {
            kReadonlyQualifier,
            kExportQualifier
        };

        Qualifier(Kind kind, std::unique_ptr<TreeNode> node);
    
        Kind kind() const;
    protected:
        Kind m_kind;
        std::unique_ptr<TreeNode> m_node;
    };

    class ReadonlyQualifier : public Qualifier {
    public:
        ReadonlyQualifier(std::unique_ptr<TreeNode> node);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void accept(TreeVisitor* visitor) override;
    };

    class ExportQualifier : public Qualifier {
    public:
        ExportQualifier(std::unique_ptr<TreeNode> node);

        std::unique_ptr<TreeNode> clone() override;

        void print(std::ostream& stream, size_t ident) override;

        void accept(TreeVisitor* visitor) override;
    };
}