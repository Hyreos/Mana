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
}