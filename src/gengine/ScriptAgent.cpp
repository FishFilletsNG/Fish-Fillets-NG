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
#include "Path.h"
#include "SimpleMsg.h"
#include "IntMsg.h"
#include "StringMsg.h"
#include "MessagerAgent.h"
#include "UnknownMsgException.h"
#include "ScriptException.h"
#include "OptionAgent.h"
#include "ScriptState.h"

#include "def-script.h"
#include "options-script.h"


//-----------------------------------------------------------------
    void
ScriptAgent::own_init()
{
    m_state = new ScriptState();
    m_state->registerFunc("sendMsg", script_options_sendMsg);
    m_state->registerFunc("setParam", script_options_setParam);
    m_state->registerFunc("getParam", script_options_getParam);
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


