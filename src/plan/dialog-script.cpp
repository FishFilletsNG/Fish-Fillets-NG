/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "dialog-script.h"

class Actor;

#include "Log.h"
#include "Path.h"
#include "FishDialog.h"
#include "DialogAgent.h"
#include "SubTitleAgent.h"
#include "SoundAgent.h"
#include "Planner.h"

#include "def-script.h"

//-----------------------------------------------------------------
    inline Planner *
getPlanner(lua_State *L)
{
    return static_cast<Planner*>(script_getLeader(L));
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

    getPlanner(L)->scriptInclude(Path::dataReadPath(filename));
    END_NOEXCEPTION;
    //NOTE: return how many values want to return to lua
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


//-----------------------------------------------------------------
/**
 * void game_planAction(func)
 */
    int
script_game_planAction(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    luaL_checktype(L, 1, LUA_TFUNCTION);
    int funcRef = luaL_ref(L, LUA_REGISTRYINDEX);

    getPlanner(L)->planAction(funcRef);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * bool game_isPlanning()
 */
    int
script_game_isPlanning(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    bool planning = getPlanner(L)->isPlanning();
    lua_pushboolean(L, planning);
    END_NOEXCEPTION;
    //NOTE: return planning
    return 1;
}

//-----------------------------------------------------------------
/**
 * bool dialog_empty()
 */
    int
script_dialog_empty(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    bool empty = DialogAgent::agent()->empty();
    lua_pushboolean(L, empty);
    END_NOEXCEPTION;
    //NOTE: return empty
    return 1;
}
//-----------------------------------------------------------------
/**
 * void dialog_addFont(fontname, file)
 */
    int
script_dialog_addFont(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *name = luaL_checkstring(L, 1);
    const char *file = luaL_checkstring(L, 2);

    SubTitleAgent::agent()->addFont(name, Path::dataReadPath(file));

    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void dialog_addDialog(name, lang, soundfile, fontname, subtitle)
 */
    int
script_dialog_addDialog(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *name = luaL_checkstring(L, 1);
    const char *lang = luaL_checkstring(L, 2);
    const char *soundfile = luaL_checkstring(L, 3);
    const char *fontname = luaL_optstring(L, 4, "");
    const char *subtitle = luaL_optstring(L, 5, "");

    FishDialog *dialog =
        new FishDialog(lang, soundfile, subtitle, fontname);
    DialogAgent::agent()->addDialog(name, dialog);

    END_NOEXCEPTION;
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

    Actor *model = getPlanner(L)->getActor(model_index);
    bool talking = DialogAgent::agent()->isTalking(model);

    lua_pushboolean(L, talking);
    END_NOEXCEPTION;
    //NOTE: return talking
    return 1;
}
//-----------------------------------------------------------------
/**
 * void model_planDialog(model_index, delay, name, busy=false)
 */
    int
script_model_planDialog(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    int delay = luaL_checkint(L, 2);
    const char *name = luaL_checkstring(L, 3);
    bool busy = lua_toboolean(L, 4);

    Actor *model = getPlanner(L)->getActor(model_index);
    DialogAgent::agent()->planDialog(name, delay, model, busy);

    END_NOEXCEPTION;
    return 0;
}

//-----------------------------------------------------------------
/**
 * void sound_playMusic(music_name)
 */
    int
script_sound_playMusic(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *music_name = luaL_checkstring(L, 1);

    SoundAgent::agent()->playMusic(Path::dataReadPath(music_name), NULL);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void sound_stopMusic()
 */
    int
script_sound_stopMusic(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    SoundAgent::agent()->stopMusic();
    END_NOEXCEPTION;
    return 0;
}

