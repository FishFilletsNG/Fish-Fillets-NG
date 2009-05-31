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
#include "LevelScript.h"
#include "LevelLoading.h"
#include "LevelCountDown.h"
#include "CommandQueue.h"
#include "MultiDrawer.h"

#include "Log.h"
#include "Room.h"
#include "StepCounter.h"
#include "View.h"
#include "OptionAgent.h"
#include "VideoAgent.h"
#include "ScriptException.h"
#include "LogicException.h"
#include "DemoMode.h"
#include "SoundAgent.h"
#include "SubTitleAgent.h"
#include "StepDecor.h"
#include "StatusDisplay.h"
#include "Picture.h"
#include "DialogStack.h"

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
    m_restartCounter = 1;
    m_insideUndo = false;
    m_wasDangerousMove = false;
    m_depth = depth;
    m_newRound = false;
    m_locker = new PhaseLocker();
    m_levelScript = new LevelScript(this);
    m_loading = new LevelLoading(m_levelScript);
    m_countdown = new LevelCountDown(m_levelScript);
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
    // because it uses the same script
    delete m_show;
    delete m_countdown;
    delete m_loading;
    delete m_levelScript;
    delete m_background;
    delete m_statusDisplay;
}
//-----------------------------------------------------------------
void
Level::fillStatus(LevelStatus *status)
{
    m_countdown->fillStatus(status);
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
    m_countdown->reset();
    m_loading->reset();
    //NOTE: let level first to draw and then play
    m_locker->reset();
    m_locker->ensurePhases(1);
    if (!m_insideUndo) {
        SoundAgent::agent()->stopMusic();
    }
    //TODO: escape "codename"
    m_levelScript->scriptDo("CODENAME = [[" + m_codename + "]]");
    m_levelScript->scriptInclude(m_datafile);
}
//-----------------------------------------------------------------
/**
 * Update level.
 */
    void
Level::own_updateState()
{
    m_newRound = false;
    if (m_locker->getLocked() == 0) {
        m_newRound = true;
        nextAction();
    }
    updateLevel();
    m_locker->decLock();

    if (m_countdown->countDown(this)) {
        finishLevel();
    }
}
//-----------------------------------------------------------------
    void
Level::own_pauseState()
{
    m_levelScript->killPlan();
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
    //NOTE: ensure that unwanted mouse press will not move a fish
    m_locker->ensurePhases(3);
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
 */
    void
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
 * Save state for undo.
 * Should be called after a player move,
 * but still before level script update.
 * @param oldMoves moves before the last move
 */
    void
Level::saveUndo(const std::string &oldMoves)
{
    if (oldMoves.empty()) {
        return;
    }

    if (m_levelScript->isRoom()) {
        Room *room = m_levelScript->room();
        // Forcing a new save prevents overwriting the last saved undo.
        bool forceSave = m_wasDangerousMove;
        m_wasDangerousMove = room->stepCounter()->isDangerousMove();

        std::string forceSaveValue = forceSave ? "true" : "false";
        m_levelScript->scriptDo("script_saveUndo(\""
                + oldMoves + "\"," + forceSaveValue + ")");
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
    if (m_countdown->isFinishedEnough()) {
        m_countdown->saveSolution();
        GameState *nextState = m_countdown->createNextState();
        if (nextState) {
            changeState(nextState);
        }
        else {
            quitState();
        }
    }
    else if (m_countdown->isWrongEnough()) {
        action_restart();
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
        Room *room = m_levelScript->room();
        std::string oldMoves = room->stepCounter()->getMoves();
        room->nextRound(getInput());
        // The old positions are now occupied, so check the isSolvable().
        bool wasSolvable = room->isSolvable();
        m_wasDangerousMove = m_wasDangerousMove || room->isFalling();

        unsigned int stepsAfter = room->stepCounter()->getStepCount();
        if (wasSolvable && stepsAfter != oldMoves.size()) {
            saveUndo(oldMoves);
        }
    }
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
            std::string moves =
                m_levelScript->room()->stepCounter()->getMoves();
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
    if (m_insideUndo) {
        if (m_levelScript->isRoom()) {
            m_levelScript->room()->setMoves(moves);
        }
    }
    else {
        m_loading->loadGame(moves);
    }
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
    //NOTE: The script is just overridden by itself,
    // so planned shows and undo remain after restart.
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
        m_insideUndo = false;
        m_levelScript->scriptDo("script_load()");
    }
    else {
        LOG_INFO(ExInfo("there is no file to load")
                .addInfo("file", file.getNative()));
    }
    return true;
}
//-----------------------------------------------------------------
/**
 * Naive undo by reloading the level.
 */
    bool
Level::action_undo()
{
    if (m_levelScript->isRoom()) {
        m_insideUndo = true;
        m_restartCounter--;
        action_restart();

        m_levelScript->scriptDo("script_loadUndo()");
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
    void
Level::controlMouse(const MouseStroke &button)
{
    if (m_levelScript->isRoom()) {
        m_levelScript->room()->controlMouse(button);
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
    room->addDecor(new StepDecor(room->stepCounter()));
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
//-----------------------------------------------------------------
std::string
Level::getLevelName() const
{
    return m_desc->findLevelName(m_codename);
}
//-----------------------------------------------------------------
int
Level::getCountForSolved() const
{
    int countdown = 10;
    if (isLoading()) {
        countdown = 0;
    }
    else if (m_levelScript->dialogs()->areRunning()) {
        countdown = 30;
    }
    return countdown;
}
//-----------------------------------------------------------------
int
Level::getCountForWrong() const
{
    //NOTE: don't forget to change briefcase_help_demo too
    return 75;
}

