/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Planner.h"

#include "ScriptState.h"
#include "CommandQueue.h"

#include "ScriptCmd.h"
#include "dialog-script.h"

//-----------------------------------------------------------------
Planner::Planner()
{
    m_script = new ScriptState();
    m_plan = new CommandQueue();
    registerScriptFuncs();
}
//-----------------------------------------------------------------
    void
Planner::registerScriptFuncs()
{
    m_script->registerFunc("file_include", script_file_include);
    m_script->registerFunc("file_exists", script_file_exists);

    m_script->registerFunc("game_planAction", script_game_planAction);
    m_script->registerFunc("game_isPlanning", script_game_isPlanning);

    m_script->registerFunc("dialog_empty", script_dialog_empty);
    m_script->registerFunc("dialog_addFont", script_dialog_addFont);
    m_script->registerFunc("dialog_addDialog", script_dialog_addDialog);
    m_script->registerFunc("model_isTalking", script_model_isTalking);
    m_script->registerFunc("model_planDialog", script_model_planDialog);

    m_script->registerFunc("sound_playMusic", script_sound_playMusic);
    m_script->registerFunc("sound_stopMusic", script_sound_stopMusic);
}
//-----------------------------------------------------------------
Planner::~Planner()
{
    //NOTE: planned ScriptCmd must be removed before script
    delete m_plan;
    delete m_script;
}
//-----------------------------------------------------------------
/**
 * Include this script file.
 */
    void
Planner::scriptInclude(const Path &filename)
{
    m_script->doFile(filename);
}
//-----------------------------------------------------------------
/**
 * Execute next action.
 * @return true for finished plan
 */
    bool
Planner::finishPlan()
{
    m_plan->executeFirst();
    return m_plan->empty();
}
//-----------------------------------------------------------------
void
Planner::interruptPlan()
{
    m_plan->removeAll();
}
//-----------------------------------------------------------------
    void
Planner::planAction(int funcRef)
{
    m_plan->planCommand(new ScriptCmd(m_script, funcRef));
}
//-----------------------------------------------------------------
/**
 * Return true when there is a planned command in queue.
 */
bool
Planner::isPlanning() const
{
    return !m_plan->empty();
}

