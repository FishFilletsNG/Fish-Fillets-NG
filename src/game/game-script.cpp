/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "game-script.h"

#include "Log.h"
#include "GameAgent.h"
#include "Driver.h"
#include "KeyDriver.h"
#include "KeyControl.h"
#include "Path.h"

extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}

//-----------------------------------------------------------------
/**
 * void createRoom(width, height, picture)
 *
 *  createRoom(40, 50, "kitchen-bg.png")
 *
 * NOTE: no one exception can be passed to "C" lua code
 */
    int
script_createRoom(lua_State *L) throw()
{
    try {
        int w = luaL_checkint(L, 1);
        int h = luaL_checkint(L, 2);
        const char *picture = luaL_checkstring(L, 3);

        GameAgent::agent()->createRoom(w, h,
                Path::dataReadPath(picture));
    }
    catch (std::exception &e) {
        LOG_WARNING(ExInfo("script error - createRoom()")
                .addInfo("what", e.what()));
        luaL_error(L, e.what());
    }
    catch (...) {
        LOG_ERROR(ExInfo("script error - createRoom()"));
        luaL_error(L, "unknown exception");
    }

    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * int addModel(kind, x, y, picture, shape)
 * Return model index.
 *
 *  table = addModel("light", 10, 30, "table.bmp",
 *  "XXXXX\n"..
 *  "  X  \n"..
 *  "  X  \n")
 *
 * NOTE: no one exception can be passed to "C" lua code
 */
    int
script_addModel(lua_State *L) throw()
{
    try {
        const char *kind = luaL_checkstring(L, 1);
        int x = luaL_checkint(L, 2);
        int y = luaL_checkint(L, 3);
        const char *picture = luaL_checkstring(L, 4);
        const char *shape = luaL_checkstring(L, 5);

        int index = GameAgent::agent()->addModel(kind, V2(x, y),
                Path::dataReadPath(picture), shape);
        lua_pushnumber(L, index);
    }
    catch (std::exception &e) {
        LOG_WARNING(ExInfo("script error - addModel()")
                .addInfo("what", e.what()));
        luaL_error(L, e.what());
    }
    catch (...) {
        LOG_ERROR(ExInfo("script error - addModel()"));
        luaL_error(L, "unknown exception");
    }

    //NOTE: return how many values want to return to lua
    return 1;
}
//-----------------------------------------------------------------
/**
 * void model_addAnim(model_index, anim_name, picture)
 *
 * NOTE: no one exception can be passed to "C" lua code
 */
    int
script_model_addAnim(lua_State *L) throw()
{
    try {
        int model_index = luaL_checkint(L, 1);
        const char *anim_name = luaL_checkstring(L, 2);
        const char *picture = luaL_checkstring(L, 3);

        GameAgent::agent()->model_addAnim(model_index, anim_name, picture);
    }
    catch (std::exception &e) {
        LOG_WARNING(ExInfo("script error - model_addAnim()")
                .addInfo("what", e.what()));
        luaL_error(L, e.what());
    }
    catch (...) {
        LOG_ERROR(ExInfo("script error - model_addAnim()"));
        luaL_error(L, "unknown exception");
    }

    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_setAnim(model_index, anim_name, phase=0)
 *
 * NOTE: no one exception can be passed to "C" lua code
 */
    int
script_model_setAnim(lua_State *L) throw()
{
    try {
        int model_index = luaL_checkint(L, 1);
        const char *anim_name = luaL_checkstring(L, 2);
        int phase = luaL_optint(L, 3, 0);

        GameAgent::agent()->model_setAnim(model_index, anim_name, phase);
    }
    catch (std::exception &e) {
        LOG_WARNING(ExInfo("script error - model_setAnim()")
                .addInfo("what", e.what()));
        luaL_error(L, e.what());
    }
    catch (...) {
        LOG_ERROR(ExInfo("script error - model_setAnim()"));
        luaL_error(L, "unknown exception");
    }

    //NOTE: return how many values want to return to lua
    return 0;
}

