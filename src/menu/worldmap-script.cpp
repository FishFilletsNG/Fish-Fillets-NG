/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "worldmap-script.h"

#include "WorldBranch.h"
#include "LevelDesc.h"
#include "LevelNode.h"
#include "def-script.h"

//-----------------------------------------------------------------
    inline WorldBranch *
getWorld(lua_State *L)
{
    return dynamic_cast<WorldBranch*>(script_getLeader(L));
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
//-----------------------------------------------------------------
/**
 * void branch_addNode(parent, codename, datafile, x, y,
 *          hidden=false, poster="")
 */
    int
script_branch_addNode(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *parent = luaL_checkstring(L, 1);
    const char *codename = luaL_checkstring(L, 2);
    const char *datafile = luaL_checkstring(L, 3);
    int nodeX = luaL_checkint(L, 4);
    int nodeY = luaL_checkint(L, 5);
    bool hidden = lua_toboolean(L, 6);
    const char *poster = luaL_optstring(L, 7, "");

    LevelNode *node = new LevelNode(codename,
                Path::dataReadPath(datafile), V2(nodeX, nodeY), poster);
    getWorld(L)->addNode(parent, node, hidden);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void branch_setEnding(codename, datafile, poster="")
 */
    int
script_branch_setEnding(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *codename = luaL_checkstring(L, 1);
    const char *datafile = luaL_checkstring(L, 2);
    const char *poster = luaL_optstring(L, 3, "");

    LevelNode *node = new LevelNode(codename,
                Path::dataReadPath(datafile), V2(-1, -1), poster);
    getWorld(L)->setEnding(node);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void node_bestSolution(codename, moves, author)
 */
    int
script_node_bestSolution(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *codename = luaL_checkstring(L, 1);
    int moves = luaL_checkint(L, 2);
    const char *author = luaL_checkstring(L, 3);

    getWorld(L)->bestSolution(codename, moves, author);
    END_NOEXCEPTION;
    return 0;
}


