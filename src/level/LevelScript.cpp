/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LevelScript.h"

#include "V2.h"
#include "Room.h"
#include "ScriptState.h"

#include "ScriptCmd.h"
#include "LogicException.h"
#include "Cube.h"
#include "Unit.h"

#include "game-script.h"
#include "level-script.h"

#include <memory> // for auto_ptr

//-----------------------------------------------------------------
/**
 * Create new plan holder.
 */
LevelScript::LevelScript(Level *aLevel)
{
    m_level = aLevel;
    registerGameFuncs();
}
//-----------------------------------------------------------------
/**
 * Create ScriptCmd for show.
 * Command will be executed in this script context.
 * @param funcRef index of lua function
 * @return new command
 */
Command *
LevelScript::createCommand(int funcRef)
{
    return new ScriptCmd(m_script, funcRef);
}

//-----------------------------------------------------------------
/**
 * Update level (plan dialogs, do anim, ...).
 */
    void
LevelScript::updateScript()
{
    m_script->doString("script_update()");
    satisfyPlan();
}
//-----------------------------------------------------------------
void
LevelScript::interruptPlan()
{
    Planner::interruptPlan();
    //NOTE: checkActive is before unBusyUnits to allow script
    // make busy unwanted fishes
    room()->checkActive();
    room()->unBusyUnits();
}
//-----------------------------------------------------------------
/**
 * Add model at scene.
 * @param new_model new object
 * @param new_unit driver for the object or NULL
 * @return model index
 * @throws LogicException when room is not created yet
 */
    int
LevelScript::addModel(Cube *new_model, Unit *new_unit)
{
    std::auto_ptr<Cube> ptr_model(new_model);
    std::auto_ptr<Unit> ptr_unit(new_unit);

    ptr_model->takeDialogs(dialogs());
    return room()->addModel(ptr_model.release(), ptr_unit.release());
}
//-----------------------------------------------------------------
    Cube *
LevelScript::getModel(int model_index)
{
    return room()->getModel(model_index);
}
//-----------------------------------------------------------------
/**
 * Returns model at location.
 */
    Cube *
LevelScript::askField(const V2 &loc)
{
    return room()->askField(loc);
}

//-----------------------------------------------------------------
    void
LevelScript::addSound(const std::string &name, const Path &file)
{
    room()->addSound(name, file);
}
//-----------------------------------------------------------------
    void
LevelScript::playSound(const std::string &name, int volume)
{
    room()->playSound(name, volume);
}
//-----------------------------------------------------------------
/**
 * Register functions usable from script.
 */
    void
LevelScript::registerGameFuncs()
{
    m_script->registerFunc("game_setRoomWaves", script_game_setRoomWaves);
    m_script->registerFunc("game_addModel", script_game_addModel);
    m_script->registerFunc("game_getCycles", script_game_getCycles);
    m_script->registerFunc("game_addDecor", script_game_addDecor);
    m_script->registerFunc("game_setScreenShift", script_game_setScreenShift);
    m_script->registerFunc("game_changeBg", script_game_changeBg);
    m_script->registerFunc("game_getBg", script_game_getBg);
    m_script->registerFunc("game_checkActive", script_game_checkActive);
    m_script->registerFunc("game_setFastFalling", script_game_setFastFalling);

    m_script->registerFunc("model_addAnim", script_model_addAnim);
    m_script->registerFunc("model_runAnim", script_model_runAnim);
    m_script->registerFunc("model_setAnim", script_model_setAnim);
    m_script->registerFunc("model_useSpecialAnim", script_model_useSpecialAnim);
    m_script->registerFunc("model_countAnims", script_model_countAnims);
    m_script->registerFunc("model_setEffect", script_model_setEffect);
    m_script->registerFunc("model_getLoc", script_model_getLoc);
    m_script->registerFunc("model_getAction", script_model_getAction);
    m_script->registerFunc("model_getState", script_model_getState);
    m_script->registerFunc("model_getDir", script_model_getDir);
    m_script->registerFunc("model_getTouchDir", script_model_getTouchDir);
    m_script->registerFunc("model_isAlive", script_model_isAlive);
    m_script->registerFunc("model_isOut", script_model_isOut);
    m_script->registerFunc("model_isLeft", script_model_isLeft);
    m_script->registerFunc("model_isAtBorder", script_model_isAtBorder);
    m_script->registerFunc("model_getW", script_model_getW);
    m_script->registerFunc("model_getH", script_model_getH);
    m_script->registerFunc("model_setGoal", script_model_setGoal);
    m_script->registerFunc("model_change_turnSide",
            script_model_change_turnSide);
    m_script->registerFunc("model_change_setLocation",
            script_model_change_setLocation);
    m_script->registerFunc("model_setViewShift",
            script_model_setViewShift);
    m_script->registerFunc("model_getViewShift",
            script_model_getViewShift);
    m_script->registerFunc("model_setBusy", script_model_setBusy);
    m_script->registerFunc("model_getExtraParams", script_model_getExtraParams);
    m_script->registerFunc("model_change_setExtraParams",
            script_model_change_setExtraParams);
    m_script->registerFunc("model_equals", script_model_equals);

    m_script->registerFunc("sound_addSound", script_sound_addSound);
    m_script->registerFunc("sound_playSound", script_sound_playSound);

    registerLevelFuncs();
}
//-----------------------------------------------------------------
void
LevelScript::registerLevelFuncs()
{
    m_script->registerFunc("level_save", script_level_save);
    m_script->registerFunc("level_load", script_level_load);

    m_script->registerFunc("level_action_move", script_level_action_move);
    m_script->registerFunc("level_action_save", script_level_action_save);
    m_script->registerFunc("level_action_load", script_level_action_load);
    m_script->registerFunc("level_action_restart", script_level_action_restart);

    m_script->registerFunc("level_createRoom", script_level_createRoom);
    m_script->registerFunc("level_getRestartCounter",
            script_level_getRestartCounter);
    m_script->registerFunc("level_getDepth", script_level_getDepth);
    m_script->registerFunc("level_isNewRound", script_level_isNewRound);
    m_script->registerFunc("level_isSolved", script_level_isSolved);
    m_script->registerFunc("level_newDemo", script_level_newDemo);
    m_script->registerFunc("level_planShow", script_level_planShow);
    m_script->registerFunc("level_isShowing", script_level_isShowing);
}

