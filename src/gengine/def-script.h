#ifndef HEADER_DEF_SCRIPT_H
#define HEADER_DEF_SCRIPT_H

#include "Log.h"

#include <stdexcept>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}


//NOTE: no one exception can be passed to "C" lua code
#define BEGIN_NOEXCEPTION try {
#define END_NOEXCEPTION \
} \
catch (std::exception &e) { \
    LOG_WARNING(ExInfo("script error") \
            .addInfo("what", e.what())); \
        luaL_error(L, e.what()); \
} \
catch (...) { \
    LOG_ERROR(ExInfo("script error")); \
        luaL_error(L, "unknown exception"); \
}

inline const char *script_getLeaderName() { return "script_leader"; }
extern void *script_getLeader(lua_State *L);
extern int script_debugStack(lua_State *L);

#endif
