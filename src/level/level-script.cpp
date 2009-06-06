/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "level-script.h"

#include "Log.h"
#include "Path.h"
#include "V2.h"
#include "Level.h"
#include "LevelScript.h"
#include "Room.h"
#include "Picture.h"

#include "def-script.h"

//-----------------------------------------------------------------
    inline LevelScript *
getLevelScript(lua_State *L)
{
    return dynamic_cast<LevelScript*>(script_getLeader(L));
}
//-----------------------------------------------------------------
    inline Level *
getLevel(lua_State *L)
{
    return getLevelScript(L)->level();
}

//-----------------------------------------------------------------
/**
 * void level_save(serialized)
 */
    int
script_level_save(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *serialized = luaL_checkstring(L, 1);
    getLevel(L)->saveGame(serialized);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void level_load(moves)
 */
    int
script_level_load(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *moves = luaL_checkstring(L, 1);
    getLevel(L)->loadGame(moves);
    END_NOEXCEPTION;
    return 0;
}

//-----------------------------------------------------------------
/**
 * bool level_action_move(symbol)
 */
    int
script_level_action_move(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    size_t size;
    const char *symbol = luaL_checklstring(L, 1, &size);
    if (size != 1) {
        ExInfo error = ExInfo("bad symbol length")
            .addInfo("length", size)
            .addInfo("symbol", symbol);
        LOG_WARNING(error);
        luaL_error(L, error.what());
    }

    bool sucess = getLevel(L)->action_move(symbol[0]);
    lua_pushboolean(L, sucess);
    END_NOEXCEPTION;
    //NOTE: return sucess
    return 1;
}
//-----------------------------------------------------------------
/**
 * bool level_action_save()
 */
    int
script_level_action_save(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    bool sucess = getLevel(L)->action_save();
    lua_pushboolean(L, sucess);
    END_NOEXCEPTION;
    //NOTE: return sucess
    return 1;
}
//-----------------------------------------------------------------
/**
 * bool level_action_load()
 */
    int
script_level_action_load(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    bool sucess = getLevel(L)->action_load();
    lua_pushboolean(L, sucess);
    END_NOEXCEPTION;
    //NOTE: return sucess
    return 1;
}
//-----------------------------------------------------------------
/**
 * bool level_action_restart()
 */
    int
script_level_action_restart(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    bool sucess = getLevel(L)->action_restart(1);
    lua_pushboolean(L, sucess);
    END_NOEXCEPTION;
    //NOTE: return sucess
    return 1;
}


//-----------------------------------------------------------------
/**
 * void level_createRoom(width, height, picture)
 * Example:
 *  createRoom(40, 50, "kitchen-bg.png")
 */
    int
script_level_createRoom(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int w = luaL_checkint(L, 1);
    int h = luaL_checkint(L, 2);
    const char *picture = luaL_checkstring(L, 3);

    getLevel(L)->createRoom(w, h, picture);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * int level_getRestartCounter()
 * 
 * Returns number of attemps, starts from 1. 
 */
    int
script_level_getRestartCounter(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int counter = getLevel(L)->getRestartCounter();
    lua_pushnumber(L, counter);
    END_NOEXCEPTION;
    //NOTE: return counter
    return 1;
}
//-----------------------------------------------------------------
/**
 * int level_getDepth()
 * 
 */
    int
script_level_getDepth(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int depth = getLevel(L)->getDepth();
    lua_pushnumber(L, depth);
    END_NOEXCEPTION;
    //NOTE: return depth
    return 1;
}
//-----------------------------------------------------------------
/**
 * bool level_isNewRound()
 * 
 */
    int
script_level_isNewRound(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    bool newRound = getLevel(L)->isNewRound();
    lua_pushboolean(L, newRound);
    END_NOEXCEPTION;
    //NOTE: return newRound
    return 1;
}
//-----------------------------------------------------------------
/**
 * bool level_isSolved()
 * 
 */
    int
script_level_isSolved(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    bool solved = getLevelScript(L)->room()->isSolved();
    lua_pushboolean(L, solved);
    END_NOEXCEPTION;
    //NOTE: return solved
    return 1;
}
//-----------------------------------------------------------------
/**
 * void level_newDemo(demofile)
 */
    int
script_level_newDemo(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *demofile = luaL_checkstring(L, 1);

    getLevel(L)->newDemo(Path::dataReadPath(demofile));
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void level_planShow(func)
 */
    int
script_level_planShow(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    luaL_checktype(L, 1, LUA_TFUNCTION);
    int funcRef = luaL_ref(L, LUA_REGISTRYINDEX);

    Command *command = getLevelScript(L)->createCommand(funcRef);
    getLevel(L)->planShow(command);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * bool level_isShowing()
 */
    int
script_level_isShowing(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    bool showing = getLevel(L)->isShowing();
    lua_pushboolean(L, showing);
    END_NOEXCEPTION;
    //NOTE: return showing
    return 1;
}

