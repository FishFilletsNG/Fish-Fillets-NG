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
#include "Anim.h"
#include "Cube.h"
#include "Rules.h"
#include "Dialog.h"
#include "DialogAgent.h"

extern "C" {
#include "lualib.h"
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

//-----------------------------------------------------------------
/**
 * void path_include(filename)
 *
 * Do file in usedir or systemdir.
 */
    int
script_path_include(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *filename = luaL_checkstring(L, 1);

    GameAgent::agent()->scriptInclude(Path::dataReadPath(filename));
    END_NOEXCEPTION;

    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * void game_createRoom(width, height, picture)
 * Example:
 *  createRoom(40, 50, "kitchen-bg.png")
 */
    int
script_game_createRoom(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int w = luaL_checkint(L, 1);
    int h = luaL_checkint(L, 2);
    const char *picture = luaL_checkstring(L, 3);

    GameAgent::agent()->createRoom(w, h, Path::dataReadPath(picture));
    END_NOEXCEPTION;

    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * int game_addModel(kind, x, y, picture, shape)
 * Return model index.
 *
 *  table = addModel("light", 10, 30, "table.bmp",
 *  [[
 *  XXXXX
 *  ..X
 *  ..X
 *  ]])
 */
    int
script_game_addModel(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *kind = luaL_checkstring(L, 1);
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    const char *picture = luaL_checkstring(L, 4);
    const char *shape = luaL_checkstring(L, 5);

    int model_index = GameAgent::agent()->addModel(kind, V2(x, y),
            Path::dataReadPath(picture), shape);
    lua_pushnumber(L, model_index);
    END_NOEXCEPTION;

    //NOTE: return model_index
    return 1;
}
//-----------------------------------------------------------------
/**
 * void model_addAnim(model_index, anim_name, picture)
 */
    int
script_model_addAnim(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *anim_name = luaL_checkstring(L, 2);
    const char *picture = luaL_checkstring(L, 3);

    Cube *model = GameAgent::agent()->getModel(model_index);
    model->anim()->addAnim(anim_name, Path::dataReadPath(picture));
    END_NOEXCEPTION;
    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_addDuplexAnim(model_index, anim_name, left_picture, right_picture)
 */
    int
script_model_addDuplexAnim(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *anim_name = luaL_checkstring(L, 2);
    const char *left_picture = luaL_checkstring(L, 3);
    const char *right_picture = luaL_checkstring(L, 4);

    Cube *model = GameAgent::agent()->getModel(model_index);
    model->anim()->addDuplexAnim(anim_name,
            Path::dataReadPath(left_picture),
            Path::dataReadPath(right_picture));
    END_NOEXCEPTION;
    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_runAnim(model_index, anim_name, phase=0)
 */
    int
script_model_runAnim(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *anim_name = luaL_checkstring(L, 2);
    int phase = luaL_optint(L, 3, 0);

    Cube *model = GameAgent::agent()->getModel(model_index);
    model->anim()->runAnim(anim_name, phase);
    END_NOEXCEPTION;
    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_setAnim(model_index, anim_name, phase)
 */
    int
script_model_setAnim(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *anim_name = luaL_checkstring(L, 2);
    int phase = luaL_checkint(L, 3);

    Cube *model = GameAgent::agent()->getModel(model_index);
    model->anim()->setAnim(anim_name, phase);
    END_NOEXCEPTION;
    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_useSpecialAnim(model_index, anim_name, phase)
 *
 * Set special anim for one phase.
 */
    int
script_model_useSpecialAnim(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *anim_name = luaL_checkstring(L, 2);
    int phase = luaL_checkint(L, 3);

    Cube *model = GameAgent::agent()->getModel(model_index);
    model->anim()->useSpecialAnim(anim_name, phase);
    END_NOEXCEPTION;
    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * (x, y) model_getLoc(model_index)
 */
    int
script_model_getLoc(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);

    Cube *model = GameAgent::agent()->getModel(model_index);
    V2 loc = model->getLocation();

    lua_pushnumber(L, loc.getX());
    lua_pushnumber(L, loc.getY());
    END_NOEXCEPTION;
    //NOTE: return (x, y)
    return 2;
}

//-----------------------------------------------------------------
/**
 * string model_getAction(model_index)
 */
    int
script_model_getAction(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = GameAgent::agent()->getModel(model_index);
    std::string action = model->rules()->getAction();

    lua_pushlstring(L, action.c_str(), action.size());
    END_NOEXCEPTION;
    //NOTE: return action
    return 1;
}
//-----------------------------------------------------------------
/**
 * bool model_isAlive(model_index)
 */
    int
script_model_isAlive(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = GameAgent::agent()->getModel(model_index);
    bool alive = model->isAlive();

    lua_pushboolean(L, alive);
    END_NOEXCEPTION;
    //NOTE: return alive
    return 1;
}
//-----------------------------------------------------------------
/**
 * void dialog_addDialog(name, lang, soundfile, subtitle)
 */
    int
script_dialog_addDialog(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *name = luaL_checkstring(L, 1);
    const char *lang = luaL_checkstring(L, 2);
    const char *soundfile = luaL_checkstring(L, 3);
    const char *subtitle = luaL_checkstring(L, 4);

    Dialog *dialog = new Dialog(lang, Path::dataReadPath(soundfile), subtitle);
    DialogAgent::agent()->addDialog(name, dialog);

    END_NOEXCEPTION;
    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * bool model_isTalking(model_index)
 */
    int
script_model_isTalking(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    bool talking = DialogAgent::agent()->isTalking(model_index);

    lua_pushboolean(L, talking);
    END_NOEXCEPTION;
    //NOTE: return talking
    return 1;
}
//-----------------------------------------------------------------
/**
 * void model_planDialog(model_index, name, delay)
 */
    int
script_model_planDialog(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *name = luaL_checkstring(L, 2);
    int delay = luaL_checkint(L, 3);

    DialogAgent::agent()->planDialog(name, delay, model_index);

    END_NOEXCEPTION;
    //NOTE: return how many values want to return to lua
    return 0;
}
