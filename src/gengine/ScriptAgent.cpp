/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ScriptAgent.h"

#include "Log.h"
#include "ExInfo.h"
#include "SimpleMsg.h"
#include "IntMsg.h"
#include "StringMsg.h"
#include "MessagerAgent.h"
#include "UnknownMsgException.h"
#include "LogicException.h"
#include "Path.h"
#include "OptionAgent.h"

extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}

//-----------------------------------------------------------------
// Lua function
//-----------------------------------------------------------------
/**
 * void sendMsg(listener, msg, [value])
 */
    int
script_sendMsg(lua_State *L) throw()
{
    try {
        BaseMsg *message = NULL;
        const char *listener = luaL_checkstring(L, 1);
        const char *msg = luaL_checkstring(L, 2);
        if (lua_isstring(L, 3)) {
            const char *string_value = luaL_checkstring(L, 3);
            message = new StringMsg(listener, msg, string_value);
        }
        else if (lua_isnumber(L, 3)) {
            int int_value = luaL_checkint(L, 3);
            message = new IntMsg(listener, msg, int_value);
        }
        else {
            message = new SimpleMsg(listener, msg);
        }

        MessagerAgent::agent()->forwardNewMsg(message);
    }
    catch (std::exception &e) {
        LOG_WARNING(ExInfo("script error - sendMsg()")
                .addInfo("what", e.what()));
        luaL_error(L, e.what());
    }
    catch (...) {
        LOG_ERROR(ExInfo("script error - sendMsg()"));
        luaL_error(L, "unknown exception");
    }

    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * void setParam(name, value)
 * NOTE: no one exception can be passed to "C" lua code
 */
    int
script_setParam(lua_State *L) throw()
{
    try {
        const char *name = luaL_checkstring(L, 1);
        const char *value = luaL_checkstring(L, 2);
        OptionAgent::agent()->setParam(name, value);
    }
    catch (std::exception &e) {
        LOG_WARNING(ExInfo("script error - setParam()")
                .addInfo("what", e.what()));
        luaL_error(L, e.what());
    }
    catch (...) {
        LOG_ERROR(ExInfo("script error - setParam()"));
        luaL_error(L, "unknown exception");
    }

    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * void getParam(name)
 * NOTE: no one exception can be passed to "C" lua code
 */
    int
script_getParam(lua_State *L) throw()
{
    int result_count = 0;
    try {
        const char *name = luaL_checkstring(L, 1);
        std::string value = OptionAgent::agent()->getParam(name);
        if (false == value.empty()) {
            lua_pushlstring(L, value.c_str(), value.size());
            result_count = 1;
        }
    }
    catch (std::exception &e) {
        LOG_WARNING(ExInfo("script error - getParam()")
                .addInfo("what", e.what()));
        luaL_error(L, e.what());
    }
    catch (...) {
        LOG_ERROR(ExInfo("script error - getParam()"));
        luaL_error(L, "unknown exception");
    }

    //NOTE: return how many values want to return to lua
    return result_count;
}

//-----------------------------------------------------------------
/**
 * Available lua extension functions.
 * {name, function}
 * ...
 * {NULL, NULL}.
 */
static const luaL_reg LUA_FUNCS[] =
{
    {"sendMsg", script_sendMsg},
    {"setParam", script_setParam},
    {"getParam", script_getParam},
    {NULL, NULL} // last item
};

//-----------------------------------------------------------------
// ScriptAgent
//-----------------------------------------------------------------
    void
ScriptAgent::own_init()
{
    m_state = lua_open();
    lua_baselibopen(m_state);
    lua_strlibopen(m_state);

    for (int i = 0; LUA_FUNCS[i].func; ++i) {
        registerFunc(LUA_FUNCS[i].name, LUA_FUNCS[i].func);
    }
}
//-----------------------------------------------------------------
    void
ScriptAgent::own_shutdown()
{
    lua_close(m_state);
}
//-----------------------------------------------------------------
/**
 * Process script on stack.
 * @param error script load status
 *
 * @throws LogicException when script is bad
 */
void
ScriptAgent::callStack(int error)
{
    if (0 == error) {
        error = lua_pcall(m_state, 0, 0, 0);
    }

    if (error) {
        const char *msg = lua_tostring(m_state, -1);
        if (NULL == msg) {
            msg = "(error with no message)";
        }
        lua_pop(m_state, 1);
        throw LogicException(ExInfo("script failure")
                .addInfo("error", msg));
    }
}
//-----------------------------------------------------------------
/**
 * Process script file.
 * @param file script
 */
void
ScriptAgent::doFile(const Path &file)
{
    int error = luaL_loadfile(m_state, file.getNative().c_str());
    callStack(error);
}
//-----------------------------------------------------------------
/**
 * Process string.
 * @param input script
 */
void
ScriptAgent::doString(const std::string &input)
{
    int error = luaL_loadbuffer(m_state, input.c_str(), input.size(),
            input.c_str());
    callStack(error);
}
//-----------------------------------------------------------------
void
ScriptAgent::registerFunc(const char *name, lua_CFunction func)
{
    LOG_INFO(ExInfo("register script func")
            .addInfo("name", name));
    lua_register(m_state, name, func);
}
//-----------------------------------------------------------------
/**
 * Messages:
 * - dofile("file") ... run script
 * - console("input") ... run string
 *
 * @throws UnknownMsgException
 */
void
ScriptAgent::receiveString(const StringMsg *msg)
{
    if ("dofile" == msg->getName()) {
        doFile(Path::dataReadPath(msg->getValue()));
    }
    else if ("console" == msg->getName()) {
        doString(msg->getValue());
    }
    else {
        throw UnknownMsgException(msg);
    }
}


