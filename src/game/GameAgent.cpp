/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "GameAgent.h"

#include "StateManager.h"

#include "Log.h"
#include "WorldMap.h"
#include "InputAgent.h"
#include "KeyStroke.h"
#include "KeyBinder.h"
#include "SimpleMsg.h"
#include "IntMsg.h"
#include "Path.h"

//-----------------------------------------------------------------
    void
GameAgent::own_init()
{
    //TODO: allow set different worldmap the picture
    keyBinding();

    m_manager = new StateManager();
    WorldMap *worldmap = new WorldMap(
            Path::dataReadPath("images/menu/mapa-0.png"));
    worldmap->initWay(
            Path::dataReadPath("script/worldmap.lua"),
            Path::dataReadPath("script/worlddesc.lua"));
    m_manager->changeState(worldmap);
}
//-----------------------------------------------------------------
/**
 * Update game.
 * Play level or show menu.
 */
    void
GameAgent::own_update()
{
    m_manager->updateGame();
}
//-----------------------------------------------------------------
/**
 * Delete room.
 */
    void
GameAgent::own_shutdown()
{
    delete m_manager;
}

//-----------------------------------------------------------------
    void
GameAgent::keyBinding()
{
    BaseMsg *msg;
    KeyBinder *keyBinder = InputAgent::agent()->keyBinder();

    // fullscreen
    KeyStroke fs(SDLK_F11, KMOD_NONE);
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

