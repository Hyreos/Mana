#pragma once

#include <string>
#include <format>

#define ASAN_CRASH() *reinterpret_cast<size_t*>(0x1337) = 0xdeadbeef;

#define CHECK(COND) if (!(COND)) { ASAN_CRASH(); std::exit(1); }

using ErrorCallback_T = void(*)(const std::string& msg);

static ErrorCallback_T g_errorCallback;

#define MANA_CHECK_MAYBE_RETURN(X, MSG) if (!(X)) { if (g_errorCallback) g_errorCallback(std::format("({}:{}): {}", m_stats.lineIndex, m_stats.columnIndex,  MSG)); return {}; }

#define MANA_TRY_GET(X, PTR, MSG) PTR = X; if (!PTR) { if (g_errorCallback) g_errorCallback(MSG); return {}; }+18

#define MANA_FATAL(MSG) if (g_errorCallback) g_errorCallback(MSG); return {};

#define MANA_FATAL_NO_RETURN(MSG) if (g_errorCallback) g_errorCallback(MSG); std::exit(1);

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
}