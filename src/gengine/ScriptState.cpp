/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ScriptState.h"

#include "Log.h"
#include "ExInfo.h"
#include "ScriptException.h"
#include "Path.h"

extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}

//-----------------------------------------------------------------
ScriptState::ScriptState()
{
    m_state = lua_open();
    luaopen_base(m_state);
    luaopen_string(m_state);
    //TODO: are need others lua libs?
}
//-----------------------------------------------------------------
ScriptState::~ScriptState()
{
    lua_close(m_state);
}
//-----------------------------------------------------------------
/**
 * Process script on stack.
 * @param error script load status
 *
 * @throws ScriptException when script is bad
 */
    void
ScriptState::callStack(int error)
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
        throw ScriptException(ExInfo("script failure")
                .addInfo("error", msg));
    }
}
//-----------------------------------------------------------------
/**
 * Process script file.
 * @param file script
 */
    void
ScriptState::doFile(const Path &file)
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
ScriptState::doString(const std::string &input)
{
    int error = luaL_loadbuffer(m_state, input.c_str(), input.size(),
            input.c_str());
    callStack(error);
}
//-----------------------------------------------------------------
    void
ScriptState::registerFunc(const char *name, lua_CFunction func)
{
    LOG_INFO(ExInfo("register script func")
            .addInfo("name", name));
    lua_register(m_state, name, func);
}

