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
#include "SubTitleAgent.h"
#include "DialogAgent.h"

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
#include "Shape.h"
#include "View.h"
#include "Driver.h"
#include "KeyDriver.h"
#include "KeyControl.h"

#include "game-script.h"

//-----------------------------------------------------------------
    void
GameAgent::own_init()
{
    m_room = NULL;
    m_script = NULL;
    m_lockPhases = 0;

    keyBinding();
    newLevel();
}
//-----------------------------------------------------------------
/**
 * Update room.
 * Call script_update() every cycle.
 * Call script_nextRound() every round.
 */
    void
GameAgent::own_update()
{
    bool room_complete = false;
    if (0 == m_lockPhases) {
        if (m_room) {
            room_complete = m_room->nextRound();
        }
    }

    if (m_script) {
        m_script->doString("update()");
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
    clearRoom();
}

//-----------------------------------------------------------------
/**
 * Create new level.
 * TODO: make levels selection menu
 */
void
GameAgent::newLevel()
{
    clearRoom();

    m_script = new ScriptState();
    registerGameFuncs();

    std::string level = getNextLevel();
    m_script->doFile(Path::dataReadPath(level));
}
//-----------------------------------------------------------------
/**
 * Clear room after visit.
 */
void
GameAgent::clearRoom()
{
    SubTitleAgent::agent()->removeAll();
    DialogAgent::agent()->removeAll();

    if (m_script) {
        delete m_script;
        m_script = NULL;
    }
    if (m_room) {
        delete m_room;
        m_room = NULL;
    }

    m_lockPhases = 0;
}
//-----------------------------------------------------------------
/**
 * Returns next level name.
 */
std::string
GameAgent::getNextLevel()
{
    //TODO: make game menu with level list
    StringMsg *msg = new StringMsg(Name::SCRIPT_NAME,
                "dostring", "nextLevel()");
    try {
        MessagerAgent::agent()->forwardNewMsg(msg);
    }
    catch (ScriptException &e) {
        LOG_WARNING(e.info());
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
    Driver *newDriver = createDriver(kind, &weight, &power, &alive);
    Cube *model = new Cube(loc,
            weight, power, alive,
            newDriver, newView, newShape);

    return m_room->addModel(model);
}
//-----------------------------------------------------------------
/**
 * Driver factory.
 * @throws LogicException when kind is unkown
 */
Driver *
GameAgent::createDriver(const std::string &kind,
        Cube::eWeight *out_weight, Cube::eWeight *out_power, bool *out_alive)
{
    Driver *result = NULL;
    if ("fish_small" == kind) {
        KeyControl smallfish;
        result = new KeyDriver(smallfish);
        *out_weight = Cube::NONE;
        *out_power = Cube::LIGHT;
        *out_alive = true;
    }
    else if ("fish_big" == kind) {
        KeyControl bigfish;
        bigfish.setUp(SDLK_KP8);
        bigfish.setDown(SDLK_KP5);
        bigfish.setLeft(SDLK_KP4);
        bigfish.setRight(SDLK_KP6);
        result = new KeyDriver(bigfish);
        *out_weight = Cube::NONE;
        *out_power = Cube::HEAVY;
        *out_alive = true;
    }
    else {
        result = new Driver();
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

