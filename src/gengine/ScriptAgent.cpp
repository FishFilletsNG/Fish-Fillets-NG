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

#include "def-script.h"

//-----------------------------------------------------------------
// Lua function
//-----------------------------------------------------------------
/**
 * void sendMsg(listener, msg, [value])
 * NOTE: no one exception can be passed to "C" lua code
 */
    int
script_sendMsg(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
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

    END_NOEXCEPTION;
    //NOTE: return how many values want to return to lua
    return 0;
}
//-----------------------------------------------------------------
/**
 * void setParam(name, value)
 */
    int
script_setParam(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *name = luaL_checkstring(L, 1);
    const char *value = luaL_checkstring(L, 2);
    OptionAgent::agent()->setParam(name, value);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * string getParam(name)
 */
    int
script_getParam(lua_State *L) throw()
{
    int result_count = 0;
    BEGIN_NOEXCEPTION;
    const char *name = luaL_checkstring(L, 1);
    std::string value = OptionAgent::agent()->getParam(name);
    if (!value.empty()) {
        lua_pushlstring(L, value.c_str(), value.size());
        result_count = 1;
    }
    END_NOEXCEPTION;
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
void
ScriptAgent::doFile(const Path &file)
{
    m_state->doFile(file);
}
//-----------------------------------------------------------------
/**
 * Messages:
 * - dostring("input") ... run string
 *
 * @throws UnknownMsgException
 */
    void
ScriptAgent::receiveString(const StringMsg *msg)
{
    if (msg->equalsName("dostring")) {
        m_state->doString(msg->getValue());
    }
    else {
        throw UnknownMsgException(msg);
    }
}


