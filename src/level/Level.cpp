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
#include "DescFinder.h"
#include "PhaseLocker.h"
#include "LevelInput.h"
#include "LevelStatus.h"
#include "LevelScript.h"
#include "LevelLoading.h"
#include "CommandQueue.h"
#include "MultiDrawer.h"

#include "Log.h"
#include "Room.h"
#include "View.h"
#include "OptionAgent.h"
#include "VideoAgent.h"
#include "ScriptException.h"
#include "LogicException.h"
#include "DemoMode.h"
#include "SoundAgent.h"
#include "SubTitleAgent.h"
#include "StepDecor.h"
#include "PosterState.h"
#include "StatusDisplay.h"
#include "Picture.h"

#include <stdio.h>
#include <assert.h>

//-----------------------------------------------------------------
/**
 * Create new level.
 */
    Level::Level(const std::string &codename, const Path &datafile, int depth)
: m_codename(codename), m_datafile(datafile)
{
    m_desc = NULL;
    m_levelStatus = NULL;
    m_restartCounter = 1;
    m_countdown = -1;
    m_roomState = ROOM_RUNNING;
    m_depth = depth;
    m_newRound = false;
    m_locker = new PhaseLocker();
    m_levelScript = new LevelScript(this);
    m_loading = new LevelLoading(m_levelScript);
    m_show = new CommandQueue();
    m_background = new MultiDrawer();
    m_statusDisplay = new StatusDisplay();
    takeHandler(new LevelInput(this));
    registerDrawable(m_background);
    registerDrawable(SubTitleAgent::agent());
    registerDrawable(m_statusDisplay);
}
//-----------------------------------------------------------------
Level::~Level()
{
    own_cleanState();
    delete m_locker;
    //NOTE: m_show must be removed before levelScript
    delete m_show;
    delete m_loading;
    delete m_levelScript;
    delete m_background;
    delete m_statusDisplay;
}

//-----------------------------------------------------------------
/**
 * Start gameplay.
 * fillDesc() and fillStatus() must be called before.
 */
    void
Level::own_initState()
{
    if (NULL == m_desc) {
        throw LogicException(ExInfo("level description is NULL")
                .addInfo("codename", m_codename));
    }
    if (NULL == m_levelStatus) {
        throw LogicException(ExInfo("level status is NULL")
                .addInfo("codename", m_codename));
    }
    m_levelStatus->setRunning(true);
    SoundAgent::agent()->stopMusic();
    m_countdown = -1;
    m_roomState = ROOM_RUNNING;
    m_loading->reset();
    //NOTE: let level first to draw and then play
    m_locker->reset();
    m_locker->ensurePhases(1);
    m_levelScript->scriptInclude(m_datafile);
}
//-----------------------------------------------------------------
/**
 * Update level.
 */
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
Level::own_resumeState()
{
    if (m_levelScript->isRoom()) {
        initScreen();
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
/**
 * Loading is paused on background.
 */
    void
Level::own_noteBg()
{
    if (m_loading->isLoading() && !m_loading->isPaused()) {
        m_loading->togglePause();
    }
}
//-----------------------------------------------------------------
    void
Level::own_noteFg()
{
    initScreen();
    if (m_loading->isLoading() && m_loading->isPaused()) {
        m_loading->togglePause();
    }
}

//-----------------------------------------------------------------
bool
Level::isLoading() const
{
    return m_loading->isLoading();
}
//-----------------------------------------------------------------
void
Level::togglePause()
{
    return m_loading->togglePause();
}
//-----------------------------------------------------------------
/**
 * Process next action.
 * @return true for finished level
 */
    bool
Level::nextAction()
{
    if (isLoading()) {
        nextLoadAction();
    }
    else {
        if (isShowing()) {
            nextShowAction();
        }
        else {
            nextPlayerAction();
        }
    }

    return satisfyRoom();
}
//-----------------------------------------------------------------
/**
 * Update room goal state.
 * @return true for finished room (solved or wrong)
 */
    bool
Level::satisfyRoom()
{
    if (m_levelScript->room()->isSolved()) {
        m_roomState = ROOM_SOLVED;
    }
    else if (m_levelScript->room()->cannotMove()) {
        m_roomState = ROOM_WRONG;
    }
    else {
        m_roomState = ROOM_RUNNING;
    }

    setCountDown();
    return countDown();
}
//-----------------------------------------------------------------
    void
Level::setCountDown()
{
    if (m_countdown == -1) {
        switch (m_roomState) {
            case ROOM_SOLVED:
                if (isLoading()) {
                    m_countdown = 0;
                }
                else {
                    m_countdown = 30;
                }
                break;
            case ROOM_WRONG:
                //NOTE: don't forget to change briefcase_help_demo too
                m_countdown = 70;
                break;
            default:
                m_countdown = -1;
                break;
        }
    }
}
//-----------------------------------------------------------------
/**
 * Count for restart or end of level.
 * NOTE: will do restart when it time come
 * @return true for finished level
 */
    bool
Level::countDown()
{
    bool result = false;
    if (m_countdown > 0) {
        m_countdown--;
    }
    else {
        switch (m_roomState) {
            case ROOM_SOLVED:
                result = true;
                break;
            case ROOM_WRONG:
                action_restart();
                break;
            default:
                //NOTE: running
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

    PosterState *poster = m_levelStatus->createPoster();
    if (poster) {
        changeState(poster);
    }
    else {
        quitState();
    }
}
//-----------------------------------------------------------------
/*
 * Update room.
 * Let objects to move.
 */
    void
Level::nextPlayerAction()
{
    if (m_levelScript->isRoom()) {
         m_levelScript->room()->nextRound(getInput());
    }
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
            displaySaveStatus();
        }
        else {
            LOG_WARNING(ExInfo("cannot save game")
                    .addInfo("file", file.getNative()));
        }
    }
}
//-----------------------------------------------------------------
    void
Level::displaySaveStatus()
{
    static const int TIME = 3;
    LOG_INFO(ExInfo("game is saved")
            .addInfo("codename", m_codename));
    m_statusDisplay->displayStatus(
            new Picture(Path::dataReadPath("images/menu/status/saved.png"),
                V2(0, 0)), TIME);
}
//-----------------------------------------------------------------
/**
 * Start loading mode.
 * @param moves saved moves to load
 */
    void
Level::loadGame(const std::string &moves)
{
    m_loading->loadGame(moves);
}
//-----------------------------------------------------------------
/**
 * Start replay mode.
 * @param moves saved moves to load
 */
    void
Level::loadReplay(const std::string &moves)
{
    m_loading->loadReplay(moves);
}

//-----------------------------------------------------------------
/**
 * Load next move.
 */
    void
Level::nextLoadAction()
{
    m_loading->nextLoadAction();
    if (!isLoading()) {
        m_levelScript->scriptDo("script_loadState()");
    }
}
//-----------------------------------------------------------------
/**
 * Let show execute.
 */
    void
Level::nextShowAction()
{
    if (m_levelScript->isRoom()) {
        m_levelScript->room()->beginFall();
        m_show->executeFirst();
        m_levelScript->room()->finishRound();
    }
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
    //TODO: is ok to run the script on second time?
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
        LOG_INFO(ExInfo("bad level condition, level cannot be finished, "
                    "no save is made"));
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
        m_restartCounter--;
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
    m_background->removeAll();
    m_background->acceptDrawer(room);

    initScreen();
}
//-----------------------------------------------------------------
void
Level::initScreen()
{
    if (m_levelScript->isRoom()) {
        std::string title = m_desc->findDesc(m_codename);
        title.append(": " + m_desc->findLevelName(m_codename));

        OptionAgent *options = OptionAgent::agent();
        options->setParam("caption", title);
        options->setParam("screen_width",
                m_levelScript->room()->getW() * View::SCALE);
        options->setParam("screen_height",
                m_levelScript->room()->getH() * View::SCALE);
        VideoAgent::agent()->initVideoMode();
    }
}
//-----------------------------------------------------------------
    void
Level::newDemo(const Path &demofile)
{
    m_levelScript->interruptPlan();
    DemoMode *demo = new DemoMode(demofile);
    pushState(demo);
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

