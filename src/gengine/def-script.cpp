/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "def-script.h"

#include "Path.h"
#include "Scripter.h"

//-----------------------------------------------------------------
    Scripter *
script_getLeader(lua_State *L)
{
    lua_pushstring(L, script_getLeaderName());
    lua_rawget(L, LUA_REGISTRYINDEX);
    if (lua_isnil(L, -1)) {
        luaL_error(L, ExInfo("no leader")
                .addInfo("key", script_getLeaderName()).what());
    }
    luaL_checktype(L, -1, LUA_TLIGHTUSERDATA);
    Scripter *result = static_cast<Scripter*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    return result;
}
//-----------------------------------------------------------------
/**
 * char *debugStack(error_message)
 */
    int
script_debugStack(lua_State *L)
{
    //NOTE: stolen from "lua/ldblib.c"
    /* size of the first part of the stack */
    static const int LEVELS1 = 12;
    /* size of the second part of the stack */
    static const int LEVELS2 = 10;

    int level = 1;  /* skip level 0 (it's this function) */
    int firstpart = 1;  /* still before eventual `...' */
    lua_Debug ar;
    if (lua_gettop(L) == 0)
      lua_pushliteral(L, "");
    else if (!lua_isstring(L, 1)) return 1;  /* no string message */
    else lua_pushliteral(L, "\n");
    lua_pushliteral(L, "stack traceback:");
    while (lua_getstack(L, level++, &ar)) {
      if (level > LEVELS1 && firstpart) {
        /* no more than `LEVELS2' more levels? */
        if (!lua_getstack(L, level+LEVELS2, &ar))
          level--;  /* keep going */
        else {
          lua_pushliteral(L, "\n\t...");  /* too many levels */
          while (lua_getstack(L, level+LEVELS2, &ar))  /* find last levels */
            level++;
        }
        firstpart = 0;
        continue;
      }
      lua_pushliteral(L, "\n\t");
      lua_getinfo(L, "Snl", &ar);
      lua_pushfstring(L, "%s:", ar.short_src);
      if (ar.currentline > 0)
        lua_pushfstring(L, "%d:", ar.currentline);
      switch (*ar.namewhat) {
        case 'g':  /* global */ 
        case 'l':  /* local */
        case 'f':  /* field */
        case 'm':  /* method */
          lua_pushfstring(L, " in function `%s'", ar.name);
          break;
        default: {
          if (*ar.what == 'm')  /* main? */
            lua_pushfstring(L, " in main chunk");
          else if (*ar.what == 'C' || *ar.what == 't')
            lua_pushliteral(L, " ?");  /* C function or tail call */
          else
            lua_pushfstring(L, " in function <%s:%d>",
                               ar.short_src, ar.linedefined);
        }
      }
      lua_concat(L, lua_gettop(L));
    }
    lua_concat(L, lua_gettop(L));
    //NOTE: return debug_message
    return 1;
}

//-----------------------------------------------------------------
/**
 * void file_include(filename)
 *
 * Do file in usedir or systemdir.
 */
    int
script_file_include(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *filename = luaL_checkstring(L, 1);

    script_getLeader(L)->scriptInclude(Path::dataReadPath(filename));
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * bool file_exists(filename)
 *
 * Returns true when such file exists in userdir or systemdir.
 */
    int
script_file_exists(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *filename = luaL_checkstring(L, 1);

    bool exists = Path::dataReadPath(filename).exists();
    lua_pushboolean(L, exists);
    END_NOEXCEPTION;
    //NOTE: return exists
    return 1;
}

