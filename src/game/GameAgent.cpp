/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "GameAgent.h"

#include "Level.h"
#include "WorldMap.h"
#include "DemoMode.h"

#include "Log.h"
#include "InputAgent.h"
#include "MessagerAgent.h"
#include "KeyStroke.h"
#include "KeyBinder.h"
#include "SimpleMsg.h"
#include "IntMsg.h"
#include "StringMsg.h"
#include "Path.h"
#include "LogicException.h"
#include "ScriptException.h"
#include "UnknownMsgException.h"
#include "minmax.h"

#include "LevelNode.h"
#include "RectBinder.h"

//-----------------------------------------------------------------
    void
GameAgent::own_init()
{
    m_level = NULL;
    m_lockPhases = 0;
    m_demo = NULL;

    m_world = new WorldMap(Path::dataReadPath("images/menu/mapa-0.png"));
    m_world->initWay(
            Path::dataReadPath("script/worldmap.lua"),
            Path::dataReadPath("script/worlddesc.lua"));

    keyBinding();
    cleanLevel();
}
//-----------------------------------------------------------------
/**
 * Update game.
 * Play level or show menu.
 */
    void
GameAgent::own_update()
{
    if (m_demo) {
        if (m_demo->finishPlan()) {
            cleanDemo();
        }
    }
    else {
        if (m_level) {
            bool room_complete = false;
            if (m_lockPhases == 0) {
                room_complete = m_level->nextAction();
            }
            m_level->updateLevel();

            if (m_lockPhases > 0) {
                m_lockPhases--;
            }
            if (room_complete) {
                finishLevel();
            }
        }
        else {
            m_world->watchCursor();
        }
    }
}
//-----------------------------------------------------------------
/**
 * Delete room.
 */
    void
GameAgent::own_shutdown()
{
    if (m_demo) {
        delete m_demo;
    }
    if (m_level) {
        delete m_level;
    }
    delete m_world;
}

//-----------------------------------------------------------------
/**
 * Return current leader.
 * @throws LogicException when no planner is not ready
 */
Planner *
GameAgent::leader()
{
    if (m_demo) {
        return m_demo;
    }
    return level();
}
//-----------------------------------------------------------------
/**
 * Return current level.
 * @throws LogicException when level is not ready
 */
Level *
GameAgent::level()
{
    if (NULL == m_level) {
        throw LogicException(ExInfo("level is not ready"));
    }
    return m_level;
}
//-----------------------------------------------------------------
/**
 * Return demo planner.
 * @throws LogicException when demo is not ready
 */
DemoMode *
GameAgent::demo()
{
    if (NULL == m_demo) {
        throw LogicException(ExInfo("demo planner is not ready"));
    }
    return m_demo;
}
//-----------------------------------------------------------------
/**
 * Leave level and activate menu.
 */
void
GameAgent::cleanLevel()
{
    m_lockPhases = 0;
    if (m_level) {
        delete m_level;
        m_level = NULL;
    }
    m_world->runMenu();
}
//-----------------------------------------------------------------
/**
 * Leave demo and activate level.
 */
void
GameAgent::cleanDemo()
{
    if (m_demo) {
        delete m_demo;
        m_demo = NULL;
    }
    if (m_level) {
        m_level->activate();
    }
    else {
        m_world->runMenu();
    }
}
//-----------------------------------------------------------------
/**
 * Start newly selected level.
 */
    void
GameAgent::newLevel()
{
    if (NULL == m_level) {
        m_level = m_world->createSelected();
        if (m_level) {
            m_world->deactivate();
            m_level->runLevel();
        }
    }
}
//-----------------------------------------------------------------
void
GameAgent::newDemo(Picture *bg, const Path &demofile)
{
    m_world->deactivate();
    if (m_level) {
        m_level->deactivate();
    }

    if (m_demo) {
        delete m_demo;
        m_demo = NULL;
    }
    m_demo = new DemoMode();
    m_demo->runDemo(bg, demofile);
}

//-----------------------------------------------------------------
/**
 * Remember best solution and show game menu.
 */
void
GameAgent::finishLevel()
{
    m_world->markSolved();
    m_level->saveSolution();
    cleanLevel();
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
    BaseMsg *msg = new SimpleMsg(this, "quit");
    keyBinder->addStroke(esc, msg);

    // fullscreen
    KeyStroke fs(SDLK_F11, KMOD_NONE);
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

    // mouse click
    msg = new SimpleMsg(this, "level_selected");
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = m_world->getW();
    rect.h = m_world->getH();
    InputAgent::agent()->rectBinder()->addRect(rect, msg);
}

//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - restart ... room restart
 * - save ... game save
 * - load ... game load
 * - switch ... switch active fish
 * - level_selected ... start new level
 * - quit ... quit level or game
 *
 * @throws UnknownMsgException
 */
    void
GameAgent::receiveSimple(const SimpleMsg *msg)
{
    if (m_demo) {
        cleanDemo();
        return;
    }

    if (msg->equalsName("restart")) {
        if (m_level) {
            m_level->interruptPlan();
            m_level->action_restart();
        }
    }
    else if (msg->equalsName("load")) {
        if (m_level) {
            m_level->interruptPlan();
            m_level->action_load();
        }
    }
    else if (msg->equalsName("save")) {
        if (m_level) {
            if (!m_level->isPlanning()) {
                m_level->action_save();
            }
        }
    }
    else if (msg->equalsName("switch")) {
        if (m_level) {
            if (!m_level->isPlanning()) {
                m_level->switchFish();
            }
        }
    }
    else if (msg->equalsName("level_selected")) {
        newLevel();
    }
    else if (msg->equalsName("quit")) {
        if (m_level) {
            cleanLevel();
        }
        else {
            MessagerAgent::agent()->forwardNewMsg(
                    new SimpleMsg(Name::APP_NAME, "quit"));
        }
    }
    else {
        throw UnknownMsgException(msg);
    }
}

