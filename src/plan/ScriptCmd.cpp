/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ScriptCmd.h"

#include "ScriptState.h"

//-----------------------------------------------------------------
/**
 * Obtain script function.
 * @param script script state where to execute
 * @param funcRef index of function at registry
 */
ScriptCmd::ScriptCmd(ScriptState *script, int funcRef)
{
    m_script = script;
    m_funcRef = funcRef;
}
//-----------------------------------------------------------------
/**
 * Remove function from registry.
 */
ScriptCmd::~ScriptCmd()
{
    m_script->unref(m_funcRef);
}
//-----------------------------------------------------------------
/**
 * Return true when command has finished his work.
 * @param count number of calls
 * @return true for finish
 * @throws ScriptException when error occures
 */
bool
ScriptCmd::finish(int count)
{
    return m_script->callCommand(m_funcRef, count);
}

