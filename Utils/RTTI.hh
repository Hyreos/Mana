#pragma once

#include <type_traits>
#include <stdint.h>

template<typename T>
struct InfoStructure;

namespace mana::rtti {
    struct TypeMetadata {
        const TypeMetadata* base;

        bool match(const TypeMetadata* metadata) const;

        template<typename T>
        bool match() const
        {
            auto* type = &InfoStructure<T>::data;

            if constexpr (std::is_final_v<T>) 
                return type == this;
            else return match(type);
        }
    };

    struct Base {
        using Base_T = Base;
    };
}

template<typename T>
struct InfoStructure {
    static const mana::rtti::TypeMetadata data;
};

#define MANA_RTTI_TYPE(TYPE)                                    \
template<>                                                      \
const mana::rtti::TypeMetadata InfoStructure<TYPE>::data {      \
    &InfoStructure<TYPE::Base_T>::data                          \
};

namespace mana::rtti {
    template<typename T, typename B>
    class Castable : public B {
    public:
        using Base_T = B;
        using Type_T = T;

        template<typename CompareT>
        bool match() const {
            const auto* info = &InfoStructure<Type_T>::data;

            return info->match<std::decay_t<CompareT>>();
        }
    };
}

MANA_RTTI_TYPE(mana::rtti::Base)