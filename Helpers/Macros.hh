#pragma once

#include <string>
#include <format>

#define ASAN_CRASH() *reinterpret_cast<size_t*>(0x1337) = 0xdeadbeef;

#define CHECK(COND) if (!(COND)) { ASAN_CRASH(); std::exit(1); }

using ErrorCallback_T = void(*)(const std::string& msg);

static ErrorCallback_T g_errorCallback;

#define MONA_CHECK_MAYBE_RETURN(X, MSG) if (!(X)) { if (g_errorCallback) g_errorCallback(std::format("({}:{}): {}", m_stats.lineIndex, m_stats.columnIndex,  MSG)); return {}; }

#define MONA_TRY_GET(X, PTR, MSG) PTR = X; if (!PTR) { if (g_errorCallback) g_errorCallback(MSG); return {}; }
