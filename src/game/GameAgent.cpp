/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "GameAgent.h"

#include "Log.h"
#include "LogicException.h"
#include "minmax.h"

#include "InputAgent.h"
#include "OptionAgent.h"
#include "MessagerAgent.h"
#include "ResSoundAgent.h"

#include "KeyStroke.h"
#include "KeyBinder.h"
#include "RectBinder.h"
#include "SimpleMsg.h"
#include "IntMsg.h"
#include "StringMsg.h"
#include "Name.h"
#include "Path.h"
#include "ScriptState.h"
#include "ScriptException.h"

#include "Room.h"
#include "Unit.h"
#include "Shape.h"
#include "View.h"
#include "KeyControl.h"
#include "ControlSym.h"
#include "UnknownMsgException.h"

#include "minmax.h"
#include "LoadException.h"
#include "CommandQueue.h"
#include "ScriptCmd.h"

#include "game-script.h"

//-----------------------------------------------------------------
    void
GameAgent::own_init()
{
    m_room = NULL;
    m_script = NULL;
    m_lockPhases = 0;
    m_restartCounter = 0;
    //TODO: set codename in game menu
    m_codename = "test_save";
    m_loadedMoves = "";
    m_loadSpeed = 1;
    m_plan = new CommandQueue();

    keyBinding();
    newLevel();
}
//-----------------------------------------------------------------
/**
 * Update game.
 */
    void
GameAgent::own_update()
{
    bool room_complete = false;
    if (m_lockPhases == 0) {
        if (m_loadedMoves.empty()) {
            if (m_plan->empty()) {
                room_complete = updateRoom();
            }
            else {
                room_complete = planRoom();
            }

            if (m_script) {
                m_script->doString("script_update()");
            }
        }
        else {
            room_complete = loadMoves();
        }
    }

    if (m_lockPhases > 0) {
        --m_lockPhases;
    }

    if (room_complete) {
        //TODO: game menu
        LOG_INFO(ExInfo("gratulation, room is complete"));
        newLevel();
    }
}
//-----------------------------------------------------------------
/**
 * Delete room.
 */
    void
GameAgent::own_shutdown()
{
    cleanGame();
    delete m_plan;
}

//-----------------------------------------------------------------
/*
 * Update room.
 * Let objects to move.
 * Call script_update() every cycle.
 * @return true for finished level
 */
    bool
GameAgent::updateRoom()
{
    bool room_complete = false;
    if (m_room) {
        room_complete = m_room->nextRound();
    }

    return room_complete;
}
//-----------------------------------------------------------------
/**
 * Create new level.
 * TODO: make levels selection menu
 * @param restart whether repeat this level
 */
    void
GameAgent::newLevel()
{
    cleanGame();

    m_script = new ScriptState();
    registerGameFuncs();

    std::string level = getNextLevel(false);
    m_script->doFile(Path::dataReadPath(level));
}
//-----------------------------------------------------------------
/**
 * Clean room after visit.
 * Script is not clean, it can continue in work.
 */
    void
GameAgent::cleanRoom()
{
    //TODO: remove only ResSoundPack used by room
    ResSoundAgent::agent()->removeAll();

    if (m_room) {
        delete m_room;
        m_room = NULL;
    }

    m_lockPhases = 0;
}
//-----------------------------------------------------------------
/**
 * Clean game state.
 */
    void
GameAgent::cleanGame()
{
    cleanRoom();
    m_plan->removeAll();

    if (m_script) {
        delete m_script;
        m_script = NULL;
    }
}
//-----------------------------------------------------------------
/**
 * Returns next level name.
 * @param restart whether repeat this level
 */
    std::string
GameAgent::getNextLevel(bool restart)
{
    m_restartCounter++;
    if (false == restart) {
        //TODO: make game menu with level list
        StringMsg *msg = new StringMsg(Name::SCRIPT_NAME,
                "dostring", "nextLevel()");
        try {
            MessagerAgent::agent()->forwardNewMsg(msg);
            m_restartCounter = 0;
        }
        catch (ScriptException &e) {
            LOG_WARNING(e.info());
        }
    }

    return OptionAgent::agent()->getParam("level", "script/level.lua");
}
//-----------------------------------------------------------------
/**
 * Register functions usable from script.
 */
    void
GameAgent::registerGameFuncs()
{
    m_script->registerFunc("file_include", script_file_include);
    m_script->registerFunc("file_exists", script_file_exists);

    m_script->registerFunc("game_createRoom", script_game_createRoom);
    m_script->registerFunc("game_addModel", script_game_addModel);
    m_script->registerFunc("game_getRestartCounter",
            script_game_getRestartCounter);
    m_script->registerFunc("game_save", script_game_save);
    m_script->registerFunc("game_load", script_game_load);

    m_script->registerFunc("game_planAction", script_game_planAction);
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

    m_script->registerFunc("dialog_empty", script_dialog_empty);
    m_script->registerFunc("dialog_addFont", script_dialog_addFont);
    m_script->registerFunc("dialog_addDialog", script_dialog_addDialog);
    m_script->registerFunc("model_isTalking", script_model_isTalking);
    m_script->registerFunc("model_planDialog", script_model_planDialog);

    m_script->registerFunc("timer_getCycles", script_timer_getCycles);
    m_script->registerFunc("sound_playMusic", script_sound_playMusic);
    m_script->registerFunc("sound_addSound", script_sound_addSound);
}
//-----------------------------------------------------------------
/**
 * Check whether room is ready.
 * @throws LogicException when room is not ready
 */
    void
GameAgent::checkRoom()
{
    if (NULL == m_room) {
        throw LogicException(ExInfo("room is not ready"));
    }
}

//-----------------------------------------------------------------
/**
 * Include this script file.
 */
    void
GameAgent::scriptInclude(const Path &filename)
{
    m_script->doFile(filename);
}
//-----------------------------------------------------------------
/**
 * Create new room
 * and change screen resolution.
 */
    void
GameAgent::createRoom(int w, int h, const Path &picture)
{
    if (m_room) {
        delete m_room;
        m_room = NULL;
    }
    m_room = new Room(w, h, picture);

    OptionAgent *options = OptionAgent::agent();
    options->setParam("screen_width", w * View::SCALE);
    options->setParam("screen_height", h * View::SCALE);
}
//-----------------------------------------------------------------
/**
 * Add model at scene.
 * @param kind kind of item (i.e. "fish_big", "item_light", ...)
 * @param loc placement location
 * @param shape see Shape for format
 * @return model index
 *
 * @throws LogicException when room is not created yet
 * @throws LayoutException when shape or location is bad
 */
int
GameAgent::addModel(const std::string &kind, const V2 &loc,
        const std::string &shape)
{
    checkRoom();

    Cube::eWeight weight;
    Cube::eWeight power;
    bool alive;
    Shape *newShape = new Shape(shape);
    View *newView = new View();
    Unit *newUnit = createUnit(kind, &weight, &power, &alive);
    Cube *model = new Cube(loc,
            weight, power, alive,
            newView, newShape);

    if (newUnit) {
        newUnit->takeModel(model);
        m_room->addUnit(newUnit);
    }

    return m_room->addModel(model);
}
//-----------------------------------------------------------------
/**
 * Unit factory.
 * @return new unit for fishes or NULL
 * @throws LogicException when kind is unkown
 */
Unit *
GameAgent::createUnit(const std::string &kind,
        Cube::eWeight *out_weight, Cube::eWeight *out_power, bool *out_alive)
{
    Unit *result = NULL;
    if ("fish_small" == kind) {
        KeyControl smallfish;
        smallfish.setUp(SDLK_i);
        smallfish.setDown(SDLK_k);
        smallfish.setLeft(SDLK_j);
        smallfish.setRight(SDLK_l);
        result = new Unit(smallfish, ControlSym('u', 'd', 'l', 'r'));
        *out_weight = Cube::LIGHT;
        *out_power = Cube::LIGHT;
        *out_alive = true;
    }
    else if ("fish_big" == kind) {
        KeyControl bigfish;
        bigfish.setUp(SDLK_w);
        bigfish.setDown(SDLK_s);
        bigfish.setLeft(SDLK_a);
        bigfish.setRight(SDLK_d);
        result = new Unit(bigfish, ControlSym('U', 'D', 'L', 'R'));
        *out_weight = Cube::LIGHT;
        *out_power = Cube::HEAVY;
        *out_alive = true;
    }
    else {
        *out_power = Cube::NONE;
        *out_alive = false;
        if ("item_light" == kind) {
            *out_weight = Cube::LIGHT;
        }
        else if ("item_heavy" == kind) {
            *out_weight = Cube::HEAVY;
        }
        else if ("item_fixed" == kind) {
            *out_weight = Cube::FIXED;
        }
        else {
            throw LogicException(ExInfo("unknown model kind")
                    .addInfo("kind", kind));
        }
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Reserve game cycle for blocking animation.
 * @param count how much phases we need
 */
    void
GameAgent::ensurePhases(int count)
{
    m_lockPhases = max(m_lockPhases, count);
}

//-----------------------------------------------------------------
    void
GameAgent::keyBinding()
{
    KeyBinder *keyBinder = InputAgent::agent()->keyBinder();
    // quit
    KeyStroke esc(SDLK_ESCAPE, KMOD_NONE);
    BaseMsg *msg = new SimpleMsg(Name::APP_NAME, "quit");
    keyBinder->addStroke(esc, msg);

    // fullscreen
    KeyStroke fs(SDLK_f, KMOD_NONE);
    msg = new SimpleMsg(Name::VIDEO_NAME, "fullscreen");
    keyBinder->addStroke(fs, msg);
    // restart
    KeyStroke restart(SDLK_BACKSPACE, KMOD_NONE);
    msg = new SimpleMsg(this, "restart");
    keyBinder->addStroke(restart, msg);
    // save
    msg = new SimpleMsg(this, "save");
    keyBinder->addStroke(KeyStroke(SDLK_F2, KMOD_NONE), msg);
    // load
    msg = new SimpleMsg(this, "load");
    keyBinder->addStroke(KeyStroke(SDLK_F3, KMOD_NONE), msg);
    // switch
    msg = new SimpleMsg(this, "switch");
    keyBinder->addStroke(KeyStroke(SDLK_SPACE, KMOD_NONE), msg);

    // volume
    KeyStroke key_plus(SDLK_KP_PLUS, KMOD_NONE);
    msg = new IntMsg(Name::SOUND_NAME, "inc_volume", 10);
    keyBinder->addStroke(key_plus, msg);
    KeyStroke key_minus(SDLK_KP_MINUS, KMOD_NONE);
    msg = new IntMsg(Name::SOUND_NAME, "dec_volume", 10);
    keyBinder->addStroke(key_minus, msg);
    // log
    KeyStroke log_plus(SDLK_KP_PLUS, KMOD_RALT);
    msg = new SimpleMsg(Name::APP_NAME, "inc_loglevel");
    keyBinder->addStroke(log_plus, msg);
    KeyStroke log_minus(SDLK_KP_MINUS, KMOD_RALT);
    msg = new SimpleMsg(Name::APP_NAME, "dec_loglevel");
    keyBinder->addStroke(log_minus, msg);
}

//-----------------------------------------------------------------
    Cube *
GameAgent::getModel(int model_index)
{
    checkRoom();
    return m_room->getModel(model_index);
}
//-----------------------------------------------------------------
    Cube *
GameAgent::askField(const V2 &loc)
{
    checkRoom();
    return m_room->askField(loc);
}

//-----------------------------------------------------------------
/**
 * Write save to the file.
 * Save moves and models state.
 * @param models saved models
 */
    void
GameAgent::saveGame(const std::string &models)
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
 * Load game.
 * @param moves saved moves to load
 */
    void
GameAgent::loadGame(const std::string &moves)
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
GameAgent::loadMoves()
{
    bool room_complete = false;
    if (0 == m_lockPhases) {
        for (int i = 0; i < m_loadSpeed
                && false == m_loadedMoves.empty(); ++i)
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
    }
    return room_complete;
}
//-----------------------------------------------------------------
/**
 * Let plan execute.
 * @return true for finished level
 */
    bool
GameAgent::planRoom()
{
    bool room_complete = false;
    if (m_room) {
        m_room->beginFall();
        m_plan->executeFirst();
        room_complete = m_room->finishRound();
    }

    return room_complete;
}
//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - restart ... room restart
 * - save ... game save
 * - load ... game load
 * - switch ... switch active fish
 *
 * @throws UnknownMsgException
 */
    void
GameAgent::receiveSimple(const SimpleMsg *msg)
{
    if (msg->equalsName("restart")) {
        action_restart();
    }
    else if (msg->equalsName("save")) {
        action_save();
    }
    else if (msg->equalsName("load")) {
        action_load();
    }
    else if (msg->equalsName("switch")) {
        if (m_room) {
            m_room->switchFish();
        }
    }
    else {
        throw UnknownMsgException(msg);
    }
}

//-----------------------------------------------------------------
    void
GameAgent::planAction(int funcRef)
{
    m_plan->planCommand(new ScriptCmd(m_script, funcRef));
}
//-----------------------------------------------------------------
/**
 * Move a fish.
 * @param symbol move symbol, e.g. 'U', 'D', 'L', 'R'
 */
    bool
GameAgent::action_move(char symbol)
{
    checkRoom();
    return m_room->makeMove(symbol);
}
//-----------------------------------------------------------------
/**
 * Save position.
 */
    void
GameAgent::action_save()
{
    if (m_script) {
        m_script->doString("script_save()");
    }
}
//-----------------------------------------------------------------
/**
 * Load position.
 */
    void
GameAgent::action_load()
{
    if (m_script) {
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
    }
}
//-----------------------------------------------------------------
void
GameAgent::action_restart()
{
    cleanRoom();
    std::string level = getNextLevel(true);
    m_script->doFile(Path::dataReadPath(level));
}

