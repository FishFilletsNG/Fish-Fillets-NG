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
#include "ScriptException.h"
#include "Path.h"
#include "OptionAgent.h"
#include "ScriptState.h"

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
// ScriptAgent
//-----------------------------------------------------------------
    void
ScriptAgent::own_init()
{
    m_state = new ScriptState();
    m_state->registerFunc("sendMsg", script_sendMsg);
    m_state->registerFunc("setParam", script_setParam);
    m_state->registerFunc("getParam", script_getParam);
}
//-----------------------------------------------------------------
    void
ScriptAgent::own_shutdown()
{
    delete m_state;
}
//-----------------------------------------------------------------
    void
ScriptAgent::registerFunc(const char *name, lua_CFunction func)
{
    m_state->registerFunc(name, func);
}
//-----------------------------------------------------------------
/**
 * Messages:
 * - dofile("file") ... run script
 * - doall("file") ... run both system and user script, no one is required
 * - console("input") ... run string
 *
 * @throws UnknownMsgException
 */
    void
ScriptAgent::receiveString(const StringMsg *msg)
{
    if (msg->equalsName("dofile")) {
        m_state->doFile(Path::dataReadPath(msg->getValue()));
    }
    else if (msg->equalsName("doall")) {
        try {
            m_state->doFile(Path::dataSystemPath(msg->getValue()));
            m_state->doFile(Path::dataUserPath(msg->getValue()));
        }
        catch (BaseException &e) {
            LOG_INFO(e.info());
        }
    }
    else if (msg->equalsName("console")) {
        m_state->doString(msg->getValue());
    }
    else {
        throw UnknownMsgException(msg);
    }
}


