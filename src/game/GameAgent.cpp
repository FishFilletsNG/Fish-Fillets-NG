/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "GameAgent.h"

#include "LogicException.h"

#include "SoundAgent.h"
#include "InputAgent.h"
#include "VideoAgent.h"
#include "OptionAgent.h"
#include "ScriptAgent.h"
#include "MessagerAgent.h"

#include "KeyStroke.h"
#include "KeyBinder.h"
#include "RectBinder.h"
#include "SimpleMsg.h"
#include "IntMsg.h"
#include "StringMsg.h"
#include "Name.h"
#include "Path.h"

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
    m_lockPhases = 0;

    SoundAgent::agent()->playMusic(
            Path::dataReadPath("music/tuxi.ogg"), NULL);
    keyBinding();

    registerGameFuncs();

    //TODO: make levels selection menu
    //TODO: make separate lua state for every level
    std::string level = OptionAgent::agent()->getParam("level",
            "script/level.lua");
    BaseMsg *msg = new StringMsg(Name::SCRIPT_NAME,
            "dofile", level);
    MessagerAgent::agent()->forwardNewMsg(msg);
}
//-----------------------------------------------------------------
/**
 * Update room.
 */
    void
GameAgent::own_update()
{
    if (0 == m_lockPhases) {
        if (m_room) {
            m_room->nextRound();
        }
    }

    if (m_lockPhases > 0) {
        --m_lockPhases;
    }
}
//-----------------------------------------------------------------
/**
 * Delete room.
 */
    void
GameAgent::own_shutdown()
{
    if (m_room) {
        delete m_room;
    }
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
 * @param picture transparent image
 * @param shape see Shape for format
 * @return model index
 *
 * @throws LogicException when room is not created yet
 * @throws LayoutException when shape or location is bad
 */
int
GameAgent::addModel(const std::string &kind,
        const V2 &loc, const Path &picture,
        const std::string &shape)
{
    checkRoom();

    Cube::eWeight weight;
    Cube::eWeight power;
    bool alive;
    Shape *newShape = new Shape(shape);
    View *newView = new View(picture);
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
        *out_weight = Cube::LIGHT;
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
        *out_weight = Cube::LIGHT;
        *out_power = Cube::HEAVY;
        *out_alive = true;
    }
    else {
        result = new Driver();
        *out_power = Cube::LIGHT;
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
    //TODO: offer MAX macro
    m_lockPhases = m_lockPhases > count ? m_lockPhases : count;
}
//-----------------------------------------------------------------
/**
 * Register functions usable from script.
 */
    void
GameAgent::registerGameFuncs()
{
    ScriptAgent *script = ScriptAgent::agent();
    script->registerFunc("createRoom", script_createRoom);
    script->registerFunc("addModel", script_addModel);
    script->registerFunc("model_addAnim", script_model_addAnim);
    script->registerFunc("model_setAnim", script_model_setAnim);
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
    KeyStroke plus(SDLK_KP_PLUS, KMOD_NONE);
    msg = new IntMsg(Name::SOUND_NAME, "inc_volume", 10);
    keyBinder->addStroke(plus, msg);
    KeyStroke minus(SDLK_KP_MINUS, KMOD_NONE);
    msg = new IntMsg(Name::SOUND_NAME, "dec_volume", 10);
    keyBinder->addStroke(minus, msg);
    // log
    KeyStroke log_plus(SDLK_KP_PLUS, KMOD_RALT);
    msg = new SimpleMsg(Name::APP_NAME, "inc_loglevel");
    keyBinder->addStroke(log_plus, msg);
    KeyStroke log_minus(SDLK_KP_MINUS, KMOD_RALT);
    msg = new SimpleMsg(Name::APP_NAME, "dec_loglevel");
    keyBinder->addStroke(log_minus, msg);

    //TEST: rectBinding
    SDL_Rect rect;
    rect.x = 20;
    rect.y = 50;
    rect.w = 100;
    rect.h = 300;
    msg = new StringMsg(Name::SCRIPT_NAME, "dofile", "script/test.lua");
    InputAgent::agent()->rectBinder()->addRect(rect, msg);
}

//-----------------------------------------------------------------
void
GameAgent::model_addAnim(int model_index,
        const std::string &name, const std::string &picture)
{
    LOG_DEBUG(ExInfo("model_addAnim")
            .addInfo("model_index", model_index)
            .addInfo("name", name)
            .addInfo("picture", picture));
    checkRoom();
    Cube *model = m_room->getModel(model_index);
    model->view()->addAnim(name, Path::dataReadPath(picture));
}
//-----------------------------------------------------------------
void
GameAgent::model_setAnim(int model_index,
        const std::string &name, int phase)
{
    checkRoom();
    Cube *model = m_room->getModel(model_index);
    model->view()->setAnim(name, phase);
}

