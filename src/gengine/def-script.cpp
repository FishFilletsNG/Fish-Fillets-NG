/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "def-script.h"

//-----------------------------------------------------------------
    void *
script_getLeader(lua_State *L)
{
    lua_pushstring(L, script_getLeaderName());
    lua_rawget(L, LUA_REGISTRYINDEX);
    if (lua_isnil(L, -1)) {
        luaL_error(L, ExInfo("no leader")
                .addInfo("key", script_getLeaderName()).what());
    }
    luaL_checktype(L, -1, LUA_TLIGHTUSERDATA);
    void *result = lua_touserdata(L, -1);
    lua_pop(L, 1);

    return result;
}

