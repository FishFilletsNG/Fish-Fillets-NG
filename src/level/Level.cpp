/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Level.h"

#include "StateManager.h"
#include "PhaseLocker.h"
#include "LevelInput.h"
#include "LevelStatus.h"
#include "LevelScript.h"
#include "CommandQueue.h"

#include "Log.h"
#include "Room.h"
#include "View.h"
#include "OptionAgent.h"
#include "VideoAgent.h"
#include "LoadException.h"
#include "ScriptException.h"
#include "LogicException.h"
#include "DemoMode.h"
#include "SoundAgent.h"
#include "StepDecor.h"
#include "minmax.h"

#include <stdio.h>
#include <assert.h>

//-----------------------------------------------------------------
/**
 * Create new level.
 */
    Level::Level(const std::string &codename, const Path &datafile, int depth)
: m_codename(codename), m_datafile(datafile)
{
    m_levelStatus = NULL;
    m_restartCounter = 1;
    m_countdown = -1;
    m_roomState = ROOM_RUNNING;
    m_loadedMoves = "";
    m_replayMode = false;
    m_loadSpeed = 1;
    m_depth = depth;
    m_newRound = false;
    m_locker = new PhaseLocker();
    m_levelScript = new LevelScript(this);
    m_show = new CommandQueue();
    takeHandler(new LevelInput(this));
}
//-----------------------------------------------------------------
Level::~Level()
{
    own_cleanState();
    delete m_locker;
    //NOTE: m_show must be removed before levelScript
    delete m_show;
    delete m_levelScript;
}

//-----------------------------------------------------------------
/**
 * Start gameplay.
 * fillStatus() must be called before.
 */
    void
Level::own_initState()
{
    if (NULL == m_levelStatus) {
        throw LogicException(ExInfo("level status is NULL")
                .addInfo("codename", m_codename));
    }
    m_levelStatus->setRunning();
    SoundAgent::agent()->stopMusic();
    m_countdown = -1;
    m_roomState = ROOM_RUNNING;
    m_loadedMoves = "";
    m_loadSpeed = 1;
    m_replayMode = false;
    m_levelScript->scriptInclude(m_datafile);
}
//-----------------------------------------------------------------
    void
Level::own_updateState()
{
    bool finished = false;
    m_newRound = false;
    if (m_locker->getLocked() == 0) {
        m_newRound = true;
        finished = nextAction();
    }
    updateLevel();
    m_locker->decLock();

    if (finished) {
        finishLevel();
    }
}
//-----------------------------------------------------------------
    void
Level::own_pauseState()
{
    if (m_levelScript->isRoom()) {
        m_levelScript->room()->deactivate();
    }
}
//-----------------------------------------------------------------
    void
Level::own_resumeState()
{
    if (m_levelScript->isRoom()) {
        m_levelScript->room()->activate();
    }
}
//-----------------------------------------------------------------
/**
 * Clean room after visit.
 */
    void
Level::own_cleanState()
{
    m_levelScript->cleanRoom();
}

//-----------------------------------------------------------------
bool
Level::isLoading() const
{
    return !m_loadedMoves.empty() || m_replayMode;
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
    if (isLoading()) {
        room_complete = nextLoadAction();
    }
    else {
        if (isShowing()) {
            room_complete = nextShowAction();
        }
        else {
            room_complete = nextPlayerAction();
        }
    }

    return satisfyRoom(room_complete);
}
//-----------------------------------------------------------------
/**
 * Update room goal state.
 * @return true for finished room (solved or wrong)
 */
    bool
Level::satisfyRoom(bool room_complete)
{
    bool finished = false;
    if (m_roomState == ROOM_RUNNING) {
        if (room_complete) {
            m_roomState = ROOM_COMPLETE;
        }
        else if (m_levelScript->room()->cannotMove()) {
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
                //NOTE: don't forget to change briefcase_help_demo too
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
    if (!isLoading()) {
        m_levelScript->updateScript();
    }
}
//-----------------------------------------------------------------
/**
 * Finish complete level.
 * Save solution.
 */
    void
Level::finishLevel()
{
    m_levelStatus->setComplete();
    saveSolution();
    quitState();
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
    if (m_levelScript->isRoom()) {
        room_complete = m_levelScript->room()->nextRound();
    }

    return room_complete;
}

//-----------------------------------------------------------------
/**
 * Write best solution to the file.
 * Save moves and models state.
 */
    void
Level::saveSolution()
{
    std::string current_moves = m_levelScript->room()->getMoves();
    m_levelStatus->writeSolvedMoves(current_moves);
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
    if (m_levelScript->isRoom()) {
        Path file = Path::dataWritePath("saves/" + m_codename + ".lua");
        FILE *saveFile = fopen(file.getNative().c_str(), "w");
        if (saveFile) {
            std::string moves = m_levelScript->room()->getMoves();
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
 * Start replay mode.
 * @param moves saved moves to load
 */
    void
Level::loadReplay(const std::string &moves)
{
    m_loadedMoves = moves;
    m_loadSpeed = 1;
    m_replayMode = true;
}
//-----------------------------------------------------------------
/**
 * Load a few moves.
 * @return true for finished level
 * @throws LoadException for bad load
 */
    bool
Level::nextLoadAction()
{
    bool room_complete = false;
    if (m_loadedMoves.empty()) {
        m_levelScript->room()->beginFall();
        room_complete = m_levelScript->room()->finishRound();
    }
    else {
        for (int i = 0; i < m_loadSpeed
                && !m_loadedMoves.empty(); ++i)
        {
            try {
                char symbol = m_loadedMoves[0];
                m_loadedMoves.erase(0, 1);

                room_complete = m_levelScript->room()->loadMove(symbol);
            }
            catch (LoadException &e) {
                throw LoadException(ExInfo(e.info())
                        .addInfo("remain", m_loadedMoves));
            }
        }

        if (m_loadedMoves.empty()) {
            if (!m_replayMode) {
                m_levelScript->scriptDo("script_loadState()");
            }
        }
    }

    return room_complete;
}

//-----------------------------------------------------------------
/**
 * Let show execute.
 * @return true for finished level
 */
    bool
Level::nextShowAction()
{
    bool room_complete = false;
    if (m_levelScript->isRoom()) {
        m_levelScript->room()->beginFall();
        m_show->executeFirst();
        room_complete = m_levelScript->room()->finishRound();
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
    own_cleanState();
    m_restartCounter++;
    //TODO: is ok to run the script on second time
    own_initState();
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
    return m_levelScript->room()->makeMove(symbol);
}
//-----------------------------------------------------------------
/**
 * Save position.
 * @return true
 */
    bool
Level::action_save()
{
    if (m_levelScript->room()->isSolvable()) {
        m_levelScript->scriptDo("script_save()");
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
        m_levelScript->scriptInclude(file);
        m_levelScript->scriptDo("script_load()");
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
    if (m_levelScript->isRoom()) {
        m_levelScript->room()->switchFish();
    }
}
//-----------------------------------------------------------------
    void
Level::controlEvent(const KeyStroke &stroke)
{
    if (m_levelScript->isRoom()) {
        m_levelScript->room()->controlEvent(stroke);
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
    Room *room = new Room(w, h, picture, m_locker, m_levelScript);
    room->addDecor(new StepDecor(room));
    m_levelScript->takeRoom(room);

    //TODO: set with and height in one step
    OptionAgent *options = OptionAgent::agent();
    options->setParam("caption", m_desc);
    options->setParam("screen_width", w * View::SCALE);
    options->setParam("screen_height", h * View::SCALE);
    VideoAgent::agent()->initVideoMode();
}
//-----------------------------------------------------------------
/**
 * Set waves on background.
 */
    void
Level::setRoomWaves(double amplitude, double periode, double speed)
{
    m_levelScript->room()->setWaves(amplitude, periode, speed);
}
//-----------------------------------------------------------------
    void
Level::newDemo(Picture *new_bg, const Path &demofile)
{
    DemoMode *demo = new DemoMode();
    m_manager->pushState(demo);
    demo->runDemo(new_bg, demofile);
}

//-----------------------------------------------------------------
bool
Level::isShowing() const
{
    return !m_show->empty();
}
//-----------------------------------------------------------------
void
Level::interruptShow()
{
    m_show->removeAll();
}
//-----------------------------------------------------------------
void
Level::planShow(Command *new_command)
{
    m_show->planCommand(new_command);
}

