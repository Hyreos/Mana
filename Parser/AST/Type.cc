#include "Type.hh"

#include "TreeTransverser.hh"

namespace mana {
    Type::Type(Kind identifier) 
        : TreeNode(kind),
            m_kind { identifier }
    {
    }

    std::unique_ptr<TreeNode> Type::clone()
    {
        return std::make_unique<Type>(m_kind);
    }

    void Type::print(std::ostream& stream, size_t ident)
    {
        TreeNode::print(stream, ident);

        switch (m_kind) {
            case Kind::kI16: {
                stream << "i16";
            } break;

            case Kind::kI32: {
                stream << "i32";
            } break;

            case Kind::kI64: {
                stream << "i64";
            } break;

            case Kind::kU16: {
                stream << "u16";
            } break;

            case Kind::kU32: {
                stream << "u32";
            } break;

            case Kind::kU64: {
                stream << "u64";
            } break;

            case Kind::kStr: {
                stream << "str";
            } break;
        }
    }

    void Type::accept(TreeVisitor* visitor)
    {
        visitor->visitType(this);
    }
}