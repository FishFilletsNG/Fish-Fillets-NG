/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "worldmap-script.h"

#include "WorldMap.h"
#include "LevelDesc.h"
#include "def-script.h"

//-----------------------------------------------------------------
    inline WorldMap *
getWorld(lua_State *L)
{
    return static_cast<WorldMap*>(script_getLeader(L));
}

//-----------------------------------------------------------------
/**
 * void worldmap_addDesc(codename, lang, levelname, desc)
 */
    int
script_worldmap_addDesc(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *codename = luaL_checkstring(L, 1);
    const char *lang = luaL_checkstring(L, 2);
    const char *levelname = luaL_checkstring(L, 3);
    const char *desc = luaL_checkstring(L, 4);

    LevelDesc *dialog = new LevelDesc(lang, levelname, desc);
    getWorld(L)->addDesc(codename, dialog);
    END_NOEXCEPTION;
    return 0;
}

