/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Level.h"

#include "Room.h"
#include "ScriptState.h"

#include "Log.h"
#include "OptionAgent.h"
#include "LogicException.h"
#include "LoadException.h"
#include "ScriptException.h"
#include "SoundAgent.h"
#include "View.h"
#include "minmax.h"

#include "game-script.h"

#include <assert.h>

//-----------------------------------------------------------------
/**
 * Create new level.
 * Use runLevel() to start it.
 */
Level::Level(const std::string &codename, const Path &datafile, int depth)
    : m_codename(codename), m_datafile(datafile)
{
    m_room = NULL;
    m_roomState = ROOM_RUNNING;
    m_countdown = -1;
    m_restartCounter = 0;
    m_loadedMoves = "";
    m_loadSpeed = 1;
    m_depth = depth;
    registerGameFuncs();
}
//-----------------------------------------------------------------
Level::~Level()
{
    cleanRoom();
}
//-----------------------------------------------------------------
/**
 * Start gameplay.
 */
void
Level::runLevel()
{
    SoundAgent::agent()->stopMusic();
    action_restart();
}
//-----------------------------------------------------------------
/**
 * Clean room after visit.
 */
    void
Level::cleanRoom()
{
    m_loadedMoves = "";
    m_loadSpeed = 1;
    m_roomState = ROOM_RUNNING;
    m_countdown = -1;
    if (m_room) {
        delete m_room;
        m_room = NULL;
    }
}
//-----------------------------------------------------------------
void
Level::activate()
{
    if (m_room) {
        m_room->activate();
    }
}
//-----------------------------------------------------------------
void
Level::deactivate()
{
    if (m_room) {
        m_room->deactivate();
    }
}

//-----------------------------------------------------------------
/**
 * Process next action.
 * @return true for finished level
 */
bool
Level::nextAction()
{
    bool room_complete = false;
    if (m_loadedMoves.empty()) {
        if (isPlanning()) {
            room_complete = nextPlanAction();
        }
        else {
            room_complete = nextPlayerAction();
        }
    }
    else {
        room_complete = nextLoadAction();
    }

    bool finished = false;
    if (m_roomState == ROOM_RUNNING) {
        if (room_complete) {
            m_roomState = ROOM_COMPLETE;
        }
        else if (m_room->cannotMove()) {
            m_roomState = ROOM_WRONG;
        }
    }
    else {
        finished = countDown();
    }
    return finished;
}
//-----------------------------------------------------------------
/**
 * Count for restart or end of level.
 * @return true for finished level
 */
bool
Level::countDown()
{
    bool result = false;
    if (m_countdown == -1) {
        switch (m_roomState) {
            case ROOM_COMPLETE:
                //NOTE: original was m_countdown=30, but with a dialog
                m_countdown = 5;
                break;
            case ROOM_WRONG:
                //NOTE: don't forget change briefcase_help_demo too
                m_countdown = 70;
                break;
            default:
                assert(!"unknown room state for countdown");
                break;
        }
    }
    else if (m_countdown > 0) {
        m_countdown--;
    }
    else {
        switch (m_roomState) {
            case ROOM_COMPLETE:
                result = true;
                break;
            case ROOM_WRONG:
                action_restart();
                break;
            default:
                assert(!"unknown room state for countdown");
                break;
        }
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Update level (plan dialogs, do anim, ...).
 */
void
Level::updateLevel()
{
    if (m_loadedMoves.empty()) {
        m_script->doString("script_update()");
    }
}
//-----------------------------------------------------------------
/*
 * Update room.
 * Let objects to move.
 * @return true for finished level
 */
    bool
Level::nextPlayerAction()
{
    bool room_complete = false;
    if (m_room) {
        room_complete = m_room->nextRound();
    }

    return room_complete;
}
//-----------------------------------------------------------------
/**
 * Check whether room is ready.
 * @throws LogicException when room is not ready
 */
    void
Level::checkRoom()
{
    if (NULL == m_room) {
        throw LogicException(ExInfo("room is not ready"));
    }
}
//-----------------------------------------------------------------
/**
 * Create new room
 * and change screen resolution.
 */
    void
Level::createRoom(int w, int h, const Path &picture)
{
    if (m_room) {
        delete m_room;
        m_room = NULL;
    }
    m_room = new Room(w, h, picture);

    //TODO: set with and height in one step
    OptionAgent *options = OptionAgent::agent();
    options->setParam("caption", m_desc);
    options->setParam("screen_width", w * View::SCALE);
    options->setParam("screen_height", h * View::SCALE);
}
//-----------------------------------------------------------------
/**
 * Add model at scene.
 * @return model index
 * @throws LogicException when room is not created yet
 */
int
Level::addModel(Cube *model, Unit *newUnit)
{
    if (NULL == m_room) {
        delete model;
        checkRoom();
    }
    return m_room->addModel(model, newUnit);
}
//-----------------------------------------------------------------
    Actor *
Level::getActor(int model_index)
{
    return getModel(model_index);
}
//-----------------------------------------------------------------
    Cube *
Level::getModel(int model_index)
{
    checkRoom();
    return m_room->getModel(model_index);
}
//-----------------------------------------------------------------
/**
 * Returns model at location.
 */
    Cube *
Level::askField(const V2 &loc)
{
    checkRoom();
    return m_room->askField(loc);
}

//-----------------------------------------------------------------
/**
 * Write best solution to the file.
 * Save moves and models state.
 */
    void
Level::saveSolution()
{
    checkRoom();
    std::string current_moves = m_room->getMoves();

    m_loadedMoves = "";
    Path oldSolution = Path::dataReadPath("solved/" + m_codename + ".lua");
    if (oldSolution.exists()) {
        try {
            //NOTE: hack, loads old solution to the m_loadedMoves
            m_script->doString("saved_moves=nil");
            m_script->doFile(oldSolution);
            m_script->doString("script_load()");
        }
        catch (ScriptException &e) {
            LOG_INFO(e.info());
        }
    }

    if (m_loadedMoves.empty() || current_moves.size() < m_loadedMoves.size()) {
        Path file = Path::dataWritePath("solved/" + m_codename + ".lua");
        FILE *saveFile = fopen(file.getNative().c_str(), "w");
        if (saveFile) {
            fputs("\nsaved_moves = '", saveFile);
            fputs(current_moves.c_str(), saveFile);
            fputs("'\n", saveFile);
            fclose(saveFile);
        }
        else {
            LOG_WARNING(ExInfo("cannot save solution")
                    .addInfo("file", file.getNative()));
        }
    }

    m_loadedMoves = "";
}
//-----------------------------------------------------------------
/**
 * Write save to the file.
 * Save moves and models state.
 * @param models saved models
 */
    void
Level::saveGame(const std::string &models)
{
    if (m_room) {
        Path file = Path::dataWritePath("saves/" + m_codename + ".lua");
        FILE *saveFile = fopen(file.getNative().c_str(), "w");
        if (saveFile) {
            std::string moves = m_room->getMoves();
            fputs("\nsaved_moves = '", saveFile);
            fputs(moves.c_str(), saveFile);
            fputs("'\n", saveFile);

            fputs("\nsaved_models = ", saveFile);
            fputs(models.c_str(), saveFile);
            fclose(saveFile);
        }
        else {
            LOG_WARNING(ExInfo("cannot save game")
                    .addInfo("file", file.getNative()));
        }
    }
}
//-----------------------------------------------------------------
/**
 * Start loading mode.
 * @param moves saved moves to load
 */
    void
Level::loadGame(const std::string &moves)
{
    m_loadedMoves = moves;
    m_loadSpeed = min(50, max(5, m_loadedMoves.size() / 150));
}
//-----------------------------------------------------------------
/**
 * Load a few moves.
 * @return true for finished level
 * @throw LoadException for bad load
 */
    bool
Level::nextLoadAction()
{
    bool room_complete = false;
    for (int i = 0; i < m_loadSpeed
            && !m_loadedMoves.empty(); ++i)
    {
        try {
            char symbol = m_loadedMoves[0];
            m_loadedMoves.erase(0, 1);

            room_complete = m_room->loadMove(symbol);
        }
        catch (LoadException &e) {
            throw LoadException(ExInfo(e.info())
                    .addInfo("remain", m_loadedMoves));
        }
    }

    if (m_loadedMoves.empty()) {
        m_script->doString("script_loadState()");
    }
    return room_complete;
}

//-----------------------------------------------------------------
/**
 * Let plan execute.
 * @return true for finished level
 */
    bool
Level::nextPlanAction()
{
    bool room_complete = false;
    if (m_room) {
        m_room->beginFall();
        finishPlan();
        room_complete = m_room->finishRound();
    }

    return room_complete;
}
//-----------------------------------------------------------------
/**
 * (re)start room.
 * @return true
 */
    bool
Level::action_restart()
{
    cleanRoom();
    m_restartCounter++;
    m_script->doFile(m_datafile);
    return true;
}
//-----------------------------------------------------------------
/**
 * Move a fish.
 * @param symbol move symbol, e.g. 'U', 'D', 'L', 'R'
 * @return true when move is done
 */
    bool
Level::action_move(char symbol)
{
    checkRoom();
    return m_room->makeMove(symbol);
}
//-----------------------------------------------------------------
/**
 * Save position.
 * @return true
 */
    bool
Level::action_save()
{
    if (m_room->isSolvable()) {
        m_script->doString("script_save()");
    }
    else {
        LOG_INFO(ExInfo("wrong way, no save is made"));
    }
    return true;
}
//-----------------------------------------------------------------
/**
 * Load position.
 * @return true
 */
    bool
Level::action_load()
{
    Path file = Path::dataReadPath("saves/" + m_codename + ".lua");
    if (file.exists()) {
        action_restart();
        m_script->doFile(file);
        m_script->doString("script_load()");
    }
    else {
        LOG_INFO(ExInfo("there is no file to load")
                .addInfo("file", file.getNative()));
    }
    return true;
}

//-----------------------------------------------------------------
void
Level::switchFish()
{
    if (m_room) {
        m_room->switchFish();
    }
}
//-----------------------------------------------------------------
int
Level::getCycles()
{
    checkRoom();
    return m_room->getCycles();
}
//-----------------------------------------------------------------
void
Level::addSound(const std::string &name, const Path &file)
{
    checkRoom();
    m_room->addSound(name, file);
}
//-----------------------------------------------------------------
void
Level::playSound(const std::string &name, int priority)
{
    checkRoom();
    m_room->playSound(name, priority);
}
//-----------------------------------------------------------------
/**
 * Register functions usable from script.
 */
    void
Level::registerGameFuncs()
{
    m_script->registerFunc("game_createRoom", script_game_createRoom);
    m_script->registerFunc("game_addModel", script_game_addModel);
    m_script->registerFunc("game_save", script_game_save);
    m_script->registerFunc("game_load", script_game_load);

    m_script->registerFunc("game_action_move", script_game_action_move);
    m_script->registerFunc("game_action_save", script_game_action_save);
    m_script->registerFunc("game_action_load", script_game_action_load);
    m_script->registerFunc("game_action_restart", script_game_action_restart);

    m_script->registerFunc("model_addAnim", script_model_addAnim);
    m_script->registerFunc("model_addDuplexAnim", script_model_addDuplexAnim);
    m_script->registerFunc("model_runAnim", script_model_runAnim);
    m_script->registerFunc("model_setAnim", script_model_setAnim);
    m_script->registerFunc("model_useSpecialAnim", script_model_useSpecialAnim);
    m_script->registerFunc("model_getLoc", script_model_getLoc);
    m_script->registerFunc("model_getAction", script_model_getAction);
    m_script->registerFunc("model_getState", script_model_getState);
    m_script->registerFunc("model_isAlive", script_model_isAlive);
    m_script->registerFunc("model_isOut", script_model_isOut);
    m_script->registerFunc("model_isLeft", script_model_isLeft);
    m_script->registerFunc("model_getW", script_model_getW);
    m_script->registerFunc("model_getH", script_model_getH);
    m_script->registerFunc("model_setGoal", script_model_setGoal);
    m_script->registerFunc("model_change_turnSide",
            script_model_change_turnSide);
    m_script->registerFunc("model_equals", script_model_equals);

    m_script->registerFunc("game_getRestartCounter",
            script_game_getRestartCounter);
    m_script->registerFunc("game_getCycles", script_game_getCycles);
    m_script->registerFunc("game_getDepth", script_game_getDepth);
    m_script->registerFunc("game_newDemo", script_game_newDemo);

    m_script->registerFunc("sound_addSound", script_sound_addSound);
    m_script->registerFunc("sound_playSound", script_sound_playSound);
}
