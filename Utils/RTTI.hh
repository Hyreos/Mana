#pragma once

#include <type_traits>
#include <functional>
#include <stdint.h>

template<typename T>
struct InfoStructure;

namespace mana {
    struct Default {};
}

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

        const TypeMetadata* type_metadata;
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
    // primary template.
    template<class T>
    struct function_traits : function_traits<decltype(&T::operator())> {
    };

    // partial specialization for function type
    template<class R, class... Args>
    struct function_traits<R(Args...)> {
        using result_type = R;
        using argument_types = std::tuple<Args...>;
    };

    // partial specialization for function pointer
    template<class R, class... Args>
    struct function_traits<R (*)(Args...)> {
        using result_type = R;
        using argument_types = std::tuple<Args...>;
    };

    // partial specialization for std::function
    template<class R, class... Args>
    struct function_traits<std::function<R(Args...)>> {
        using result_type = R;
        using argument_types = std::tuple<Args...>;
    };

    // partial specialization for pointer-to-member-function (i.e., operator()'s)
    template<class T, class R, class... Args>
    struct function_traits<R (T::*)(Args...)> {
        using result_type = R;
        using argument_types = std::tuple<Args...>;
    };

    template<class T, class R, class... Args>
    struct function_traits<R (T::*)(Args...) const> {
        using result_type = R;
        using argument_types = std::tuple<Args...>;
    };

    template<typename T> struct remove_all_const : std::remove_const<T> {};

    template<typename T> struct remove_all_const<T*> {
        typedef typename remove_all_const<T>::type *type;
    };

    template<typename T> struct remove_all_const<T * const> {
        typedef typename remove_all_const<T>::type *type;
    };

    template<typename T>
    using remove_all_const_t = typename remove_all_const<T>::type;

    template<typename T>
    using raw_type_t = remove_all_const_t<std::remove_pointer_t<std::decay_t<T>>>;

    template<typename T, typename B>
    class Castable : public B {
    public:
        Castable()
        {
            this->type_metadata = &InfoStructure<T>::data;
        }

        template<typename CompareT>
        inline bool match() const {
            const auto* info = this->type_metadata;

            return info->template match<CompareT>();
        }
    };

    template<typename CmpT, typename T>
    inline bool match(T* obj) {
        using Type = raw_type_t<T>;

        if (obj->type_metadata == &InfoStructure<Type>::data)
            return true;

        return obj->type_metadata->template match<CmpT>();
    }

    template<typename T>
    struct is_default_match_case {
        using Func_T = std::decay_t<T>;
        using FuncTrait_T = function_traits<Func_T>;
        using FuncTraitFirstArg_T = std::tuple_element_t<0, typename FuncTrait_T::argument_types>;
        using FuncTraitFirstArgNoConstPtr_T = raw_type_t<FuncTraitFirstArg_T>;

        static constexpr bool value = std::is_same_v<FuncTraitFirstArgNoConstPtr_T, Default>;
    };

    template<typename T>
    inline constexpr bool is_default_match_case_v = is_default_match_case<T>::value;
   
    template<typename T, typename... Fn_T>
    inline auto Match(T* object, Fn_T&&... args) {
        using FirstCallbackArg_T = std::tuple_element_t<0, std::tuple<Fn_T...>>;
        using FirstCallbackArgTraits_T = function_traits<FirstCallbackArg_T>;
        using FirstCallbackArgFirstParam_T = typename std::tuple_element_t<0, typename FirstCallbackArgTraits_T::argument_types>;

        using Storage_T = std::conditional_t<   std::is_same_v<typename FirstCallbackArgTraits_T::result_type, void>, 
                                                uint8_t, 
                                                typename FirstCallbackArgTraits_T::result_type   >;

        thread_local std::array<uint8_t, sizeof(Storage_T)> buf;
        Storage_T* storageptr = reinterpret_cast<Storage_T*>(&buf[0]);

        static_assert(sizeof...(Fn_T) > 0);

        static_assert(( (is_default_match_case_v<Fn_T>) || ...), "Missing default Match case.");

        auto try_case = [&](auto&& case_fn) -> bool {
            using Func_T = std::decay_t<decltype(case_fn)>;
            using FuncTrait_T = function_traits<Func_T>;
            using FuncTraitFirstArg_T = std::tuple_element_t<0, typename FuncTrait_T::argument_types>;
            using FuncTraitFirstArgNoConstPtr_T = raw_type_t<FuncTraitFirstArg_T>;

            static_assert(
                std::is_same_v<typename FirstCallbackArgTraits_T::result_type, typename FuncTrait_T::result_type>, 
                "Function return types differ between functions."
            );

            if constexpr (!std::is_same_v<FuncTraitFirstArgNoConstPtr_T, Default>) {
                if (match<FuncTraitFirstArgNoConstPtr_T>(object)) {
                    if constexpr (!std::is_same_v<typename FuncTrait_T::result_type, void>)
                        new (storageptr) Storage_T(case_fn(static_cast<FuncTraitFirstArg_T>(object)));
                    else case_fn(static_cast<FuncTraitFirstArg_T>(object));

                    return true;
                }
            } else new (storageptr) Storage_T(case_fn(Default()));

            return false;
        };

        auto v = ((try_case(std::forward<Fn_T>(args)) || ...));

        if constexpr (!std::is_same_v<typename FirstCallbackArgTraits_T::result_type, void>) {
            return std::move(*storageptr);
        } else {
            return;
        }
    }
}
