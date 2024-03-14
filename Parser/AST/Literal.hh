#pragma once

#pragma once

#include "TreeNode.hh"

#include <string>
#include <variant>
#include <absl/types/variant.h>

namespace mana {
    class Literal : public TreeNode {
    public:
        struct TypeMask {
            enum Enum {
                kInt64Lit = 0x1,
                kInt32Lit = 0x2,
                kInt16Lit = 0x4,
                kUint64Lit = 0x8,
                kUint32Lit = 0x10,
                kUint16Lit = 0x20,
                kFp32Lit = 0x40,
                kFp64Lit = 0x80,
                kStrLit = 0x100
            };
        };

        static constexpr TreeNode::Type kind = TreeNode::Type::kLiteral;

        explicit Literal(int64_t value);

        explicit Literal(int32_t value);

        explicit Literal(int16_t value);

        explicit Literal(uint64_t value);

        explicit Literal(uint32_t value);

        explicit Literal(uint16_t value);

        explicit Literal(float value);

        explicit Literal(double value);

        double  asFp64() const;

        float   asFp32() const;

        int64_t asI64() const;

        int32_t asI32() const;

        int16_t asI16() const;

        uint64_t asU64() const;

        uint32_t asU32() const;

        uint16_t asU16() const;

        const std::string& asStr() const;
    private:
        TypeMask::Enum m_type;

        std::variant<uint64_t, double, std::string> m_value;
    };
}