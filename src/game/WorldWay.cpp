/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WorldWay.h"

#include "Log.h"
#include "LevelNode.h"
#include "ScriptState.h"
#include "LuaTable.h"

extern "C" {
#include "lauxlib.h"
}

LevelNode *WorldWay::ms_startNode = NULL;
//-----------------------------------------------------------------
/**
 * Read childern list.
 * {{ child_table }, { child_table }, ...}
 */
void
WorldWay::addChildren(lua_State *L, LevelNode *parent, int table_index)
{
    lua_pushnil(L);
    while (lua_next(L, table_index) != 0) {
        parent->addChild(parseNode(L));
        lua_pop(L, 1);
    }
}
//-----------------------------------------------------------------
/**
 * Read node table.
 * {
 *  codename = "start",
 *  datafile = "script/start/init.lua",
 *  x = 300, y = 100,
 *  hidden = false,
 *  children = {...}
 * }
 *
 * @return node or NULL
 */
LevelNode *
WorldWay::parseNode(lua_State *L)
{

    LuaTable nodeTable(L, lua_gettop(L));
    std::string codename = nodeTable.getString("codename");
    std::string datafile = nodeTable.getString("datafile");
    int x = nodeTable.getInt("x");
    int y = nodeTable.getInt("y");
    bool hidden = nodeTable.getBool("hidden");
    LuaTable children = nodeTable.pushTable("children");

    LOG_DEBUG(ExInfo("TEST: node")
            .addInfo("codename", codename)
            .addInfo("datafile", datafile)
            .addInfo("x", x)
            .addInfo("y", y)
            .addInfo("hidden", hidden));

    LevelNode *result = new LevelNode(codename,
            Path::dataReadPath(datafile), V2(x, y));
    addChildren(L, result, children.getStackIndex());
    //NOTE: remove children table
    lua_pop(L, 1);

    if (Path::dataReadPath("solved/" + codename + ".lua").exists()) {
        result->setState(LevelNode::STATE_SOLVED);
    }
    else if (hidden) {
        result->setState(LevelNode::STATE_HIDDEN);
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * void worldway_parseWay(table)
 */
    int
WorldWay::script_parseWay(lua_State *L) throw()
{
    try {
        ms_startNode = WorldWay::parseNode(L);
    }
    catch (std::exception &e) {
        LOG_WARNING(ExInfo("script error")
                .addInfo("what", e.what()));
        luaL_error(L, e.what());
    }
    catch (...) {
        LOG_ERROR(ExInfo("script error"));
        luaL_error(L, "unknown exception");
    }

    return 0;
}
//-----------------------------------------------------------------
LevelNode*
WorldWay::createWay(const Path &file)
{
    ms_startNode = NULL;
    ScriptState script;
    script.registerFunc("worldway_parseWay", script_parseWay);
    script.doFile(file);
    if (ms_startNode && ms_startNode->getState() < LevelNode::STATE_OPEN) {
        ms_startNode->setState(LevelNode::STATE_OPEN);
    }
    return ms_startNode;
}

