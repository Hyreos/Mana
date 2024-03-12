#pragma once

#include <string>
#include <functional>
#include <cstdint>
#include <type_traits>
#include <limits>
#include <iostream>

#define ASAN_CRASH() *reinterpret_cast<size_t*>(0x1337) = 0xdeadbeef;

#define CHECK(COND) if (!(COND)) {  ASAN_CRASH(); std::exit(1); }

using ErrorCallback_T = void(*)(const std::string& msg);

inline ErrorCallback_T g_errorCallback;

#define MANA_CHECK_MAYBE_RETURN(X, MSG) { if (!(X)) { if (g_errorCallback) g_errorCallback(std::string("(") + std::to_string(m_stats.lineIndex) + ":" + std::to_string(m_stats.columnIndex) + "): " + MSG); return {}; } }

#define MANA_TRY_GET(X, PTR, MSG) { PTR = X; if (!PTR) { if (g_errorCallback) g_errorCallback(MSG); return {}; } }

#define MANA_FATAL(MSG) { if (g_errorCallback) g_errorCallback(MSG); return {} };

#define MANA_FATAL_NO_RETURN(MSG) { if (g_errorCallback) g_errorCallback(MSG); std::exit(1); }

namespace mana {
    [[noreturn]] inline void unreachable() {
    // Uses compiler specific extensions if possible.
    // Even if no extension is used, undefined behavior is still raised by
    // an empty function body and the noreturn attribute.
    #if defined(_MSC_VER) && !defined(__clang__) // MSVC
        __assume(false);
    #else // GCC, Clang
        __builtin_unreachable();
    #endif
    }

    inline std::string_view unescapeCharacter(char c)
    {
        switch (c)
        {
            case '\a':  return "\\a";
            case '\b':  return "\\b";
            case '\f':  return "\\f";
            case '\n':  return "\\n";
            case '\r':  return "\\r";
            case '\t':  return "\\t";
            case '\v':  return "\\v";
            case '\\':  return "\\\\";
            case '\'':  return "\\'";
            case '\"':  return "\\\"";
            case '\?':  return "\\\?";
            default: return "";
        }

        unreachable();
    }

    // https://en.cppreference.com/w/cpp/utility/in_range
    template<class T, class U>
    constexpr bool cmp_equal(T t, U u) noexcept
    {
        if constexpr (std::is_signed_v<T> == std::is_signed_v<U>)
            return t == u;
        else if constexpr (std::is_signed_v<T>)
            return t >= 0 && std::make_unsigned_t<T>(t) == u;
        else
            return u >= 0 && std::make_unsigned_t<U>(u) == t;
    }
    
    template<class T, class U>
    constexpr bool cmp_not_equal(T t, U u) noexcept
    {
        return !cmp_equal(t, u);
    }
    
    template<class T, class U>
    constexpr bool cmp_less(T t, U u) noexcept
    {
        if constexpr (std::is_signed_v<T> == std::is_signed_v<U>)
            return t < u;
        else if constexpr (std::is_signed_v<T>)
            return t < 0 || std::make_unsigned_t<T>(t) < u;
        else
            return u >= 0 && t < std::make_unsigned_t<U>(u);
    }
    
    template<class T, class U>
    constexpr bool cmp_greater(T t, U u) noexcept
    {
        return cmp_less(u, t);
    }
    
    template<class T, class U>
    constexpr bool cmp_less_equal(T t, U u) noexcept
    {
        return !cmp_less(u, t);
    }
    
    template<class T, class U>
    constexpr bool cmp_greater_equal(T t, U u) noexcept
    {
        return !cmp_less(t, u);
    }

    template<class R, class T>
    constexpr bool in_range(T t) noexcept
    {
        return cmp_greater_equal(t, std::numeric_limits<R>::min()) &&
            cmp_less_equal(t, std::numeric_limits<R>::max());
    }
}