/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Scripter.h"

#include "ScriptState.h"

#include "def-script.h"

//-----------------------------------------------------------------
Scripter::Scripter()
{
    m_script = new ScriptState();
    m_script->registerLeader(this);

    m_script->registerFunc("file_include", script_file_include);
    m_script->registerFunc("file_exists", script_file_exists);
}
//-----------------------------------------------------------------
Scripter::~Scripter()
{
    delete m_script;
}
//-----------------------------------------------------------------
/**
 * Include this script file.
 */
    void
Scripter::scriptInclude(const Path &filename)
{
    m_script->doFile(filename);
}
//-----------------------------------------------------------------
/**
 * Run this command.
 */
    void
Scripter::scriptDo(const std::string &input)
{
    m_script->doString(input);
}

