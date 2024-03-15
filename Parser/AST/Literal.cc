#include "Literal.hh"

#include "TreeTransverser.hh"

#include "Helpers/Macros.hh"

namespace mana::ast {
    Literal::Literal(int64_t value)
        : TreeNode(baseType),
            m_value{ static_cast<uint64_t>(value) },
            m_type{ TypeMask::kInt64Lit }
    {
    }

    Literal::Literal(int32_t value)
        : TreeNode(baseType),
            m_value{ static_cast<uint64_t>(value) },
            m_type{ TypeMask::kInt32Lit }
    {
    }

    Literal::Literal(int16_t value)
        : TreeNode(baseType),
            m_value{ static_cast<uint64_t>(value) },
            m_type{ TypeMask::kInt16Lit }
    {
    }

    Literal::Literal(uint64_t value)
        : TreeNode(baseType),
            m_value{ value },
            m_type{ TypeMask::kUint64Lit }
    {
    }

    Literal::Literal(uint32_t value)
        : TreeNode(baseType),
            m_value{ static_cast<uint64_t>(value) },
            m_type{ TypeMask::kUint32Lit }
    {
    }

    Literal::Literal(uint16_t value)
        : TreeNode(baseType),
            m_value{ static_cast<uint64_t>(value) },
            m_type{ TypeMask::kUint16Lit }
    {
    }

    Literal::Literal(double value)
        : TreeNode(baseType),
            m_value{ value },
            m_type{ TypeMask::kFp64Lit }
    {
    }

    Literal::Literal(float value)
        : TreeNode(baseType),
            m_value{ static_cast<double>(value) },
            m_type{ TypeMask::kFp32Lit }
    {
    }

    Literal::Literal(const std::string& str)
        : TreeNode(baseType),
            m_value{ str },
            m_type{ TypeMask::kStrLit }
    {
    }

    double Literal::asFp64() const
    {
        MANA_CHECK_NO_RETURN(m_type & (TypeMask::kFp64Lit | TypeMask::kFp32Lit), "Literal is not a floating point value.");

        return std::get<double>(m_value);
    }

    float Literal::asFp32() const
    {
        MANA_CHECK_NO_RETURN(m_type & (TypeMask::kFp32Lit), "Literal is not a floating point value.");

        return static_cast<float>(std::get<double>(m_value));
    }

    int64_t Literal::asI64() const
    {
        MANA_CHECK_NO_RETURN(m_type & (TypeMask::kInt16Lit | TypeMask::kInt32Lit | TypeMask::kInt64Lit), "Value is not convertible to i32.");

        return static_cast<int32_t>(std::get<uint64_t>(m_value));
    }

    int32_t Literal::asI32() const
    {
        MANA_CHECK_NO_RETURN(m_type & (TypeMask::kInt16Lit | TypeMask::kInt32Lit), "Value is not convertible to i32.");

        return static_cast<int32_t>(std::get<uint64_t>(m_value));
    }

    int16_t Literal::asI16() const
    {
        MANA_CHECK_NO_RETURN(m_type & TypeMask::kInt16Lit, "Value is not convertible to i32.");

        return static_cast<int16_t>(std::get<uint64_t>(m_value));
    }

    uint64_t Literal::asU64() const
    {
        MANA_CHECK_NO_RETURN(m_type & (TypeMask::kUint16Lit | TypeMask::kUint32Lit | TypeMask::kUint64Lit), "Value is not convertible to i32.");

        return static_cast<uint32_t>(std::get<uint64_t>(m_value));
    }

    uint32_t Literal::asU32() const
    {
        MANA_CHECK_NO_RETURN(m_type & (TypeMask::kUint16Lit | TypeMask::kUint32Lit), "Value is not convertible to i32.");

        return static_cast<uint32_t>(std::get<uint64_t>(m_value));
    }

    uint16_t Literal::asU16() const
    {
        MANA_CHECK_NO_RETURN(m_type & TypeMask::kUint16Lit, "Value is not convertible to i32.");

        return static_cast<uint16_t>(std::get<uint64_t>(m_value));
    }

    const std::string& Literal::asStr() const
    {
        MANA_CHECK_NO_RETURN(m_type & TypeMask::kStrLit, "Value is not convertible to a string.");

        return std::get<std::string>(m_value);
    }
}