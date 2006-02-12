/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ScriptAgent.h"

#include "ScriptState.h"
#include "StringMsg.h"
#include "UnknownMsgException.h"

#include "options-script.h"


//-----------------------------------------------------------------
    void
ScriptAgent::own_init()
{
    registerFunc("sendMsg", script_options_sendMsg);
    registerFunc("setParam", script_options_setParam);
    registerFunc("getParam", script_options_getParam);
}
//-----------------------------------------------------------------
    void
ScriptAgent::registerFunc(const char *name, lua_CFunction func)
{
    m_script->registerFunc(name, func);
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
        scriptDo(msg->getValue());
    }
    else {
        throw UnknownMsgException(msg);
    }
}


