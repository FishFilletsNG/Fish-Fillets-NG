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

#include "Log.h"
#include "InputAgent.h"
#include "OptionAgent.h"
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

//-----------------------------------------------------------------
    void
GameAgent::own_init()
{
    m_level = NULL;
    m_lockPhases = 0;

    keyBinding();
    newLevel();
}
//-----------------------------------------------------------------
/**
 * Update game.
 * Let objects move.
 * Call script_update() every cycle.
 */
    void
GameAgent::own_update()
{
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
            //TODO: game menu
            LOG_INFO(ExInfo("gratulation, room is complete"));
            newLevel();
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
    cleanLevel();
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
void
GameAgent::cleanLevel()
{
    m_lockPhases = 0;
    if (m_level) {
        delete m_level;
        m_level = NULL;
    }
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
    cleanLevel();
    //FIXME: set codename
    std::string codename = "test_save";
    std::string datafile = getNextLevel();
    m_level = new Level(codename, Path::dataReadPath(datafile));
    m_level->action_restart();
}
//-----------------------------------------------------------------
/**
 * Returns next level name.
 * NOTE: this function is temporary, there will be game menu
 */
    std::string
GameAgent::getNextLevel()
{
    //TODO: make game menu with level list
    try {
        StringMsg *msg = new StringMsg(Name::SCRIPT_NAME,
                "dostring", "nextLevel()");
        MessagerAgent::agent()->forwardNewMsg(msg);
    }
    catch (ScriptException &e) {
        LOG_WARNING(e.info());
    }

    return OptionAgent::agent()->getParam("level",
            "script/our_level_name/init.lua");
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
        if (m_level) {
            m_level->action_restart();
        }
    }
    else if (msg->equalsName("save")) {
        if (m_level) {
            m_level->action_save();
        }
    }
    else if (msg->equalsName("load")) {
        if (m_level) {
            m_level->action_load();
        }
    }
    else if (msg->equalsName("switch")) {
        if (m_level) {
            m_level->switchFish();
        }
    }
    else {
        throw UnknownMsgException(msg);
    }
}

