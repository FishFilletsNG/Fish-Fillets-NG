/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LuaTable.h"

#include "ExInfo.h"

extern "C" {
#include "lauxlib.h"
}

//-----------------------------------------------------------------
LuaTable::LuaTable(lua_State *state, int stackIndex)
{
    m_state = state;
    m_stackIndex = stackIndex;
    if (stackIndex < 0) {
        luaL_error(m_state, ExInfo("table stack index must be positive")
                .addInfo("stackIndex", stackIndex).what());
    }
}
//-----------------------------------------------------------------
std::string
LuaTable::getString(const std::string &key) const
{
    lua_pushlstring(m_state, key.c_str(), key.size());
    lua_rawget(m_state, m_stackIndex);
    if (lua_isnil(m_state, -1)) {
        luaL_error(m_state, ExInfo("no such value")
                .addInfo("key", key).what());
    }
    std::string result = luaL_checkstring(m_state, -1);
    lua_pop(m_state, 1);
    return result;
}
//-----------------------------------------------------------------
int
LuaTable::getInt(const std::string &key) const
{
    lua_pushlstring(m_state, key.c_str(), key.size());
    lua_rawget(m_state, m_stackIndex);
    if (lua_isnil(m_state, -1)) {
        luaL_error(m_state, ExInfo("no such value")
                .addInfo("key", key).what());
    }
    int result = luaL_checkint(m_state, -1);
    lua_pop(m_state, 1);
    return result;
}
//-----------------------------------------------------------------
bool
LuaTable::getBool(const std::string &key) const
{
    lua_pushlstring(m_state, key.c_str(), key.size());
    lua_rawget(m_state, m_stackIndex);
    if (lua_isnil(m_state, -1)) {
        luaL_error(m_state, ExInfo("no such value")
                .addInfo("key", key).what());
    }
    bool result = lua_toboolean(m_state, -1);
    lua_pop(m_state, 1);
    return result;
}
//-----------------------------------------------------------------
/**
 * Find table at key and push it at stack.
 */
LuaTable
LuaTable::pushTable(const std::string &key) const
{
    lua_pushlstring(m_state, key.c_str(), key.size());
    lua_rawget(m_state, m_stackIndex);
    if (!lua_istable(m_state, -1)) {
        luaL_error(m_state, ExInfo("type is not table")
                .addInfo("type",
                    lua_typename(m_state, lua_type(m_state, -1))).what());
    }
    return LuaTable(m_state, lua_gettop(m_state));
}

