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
#include "OptionAgent.h"

#include "SDL.h"

//-----------------------------------------------------------------
    void
GameAgent::own_init()
{
    m_manager = new StateManager();
    Path pathMap = Path::dataReadPath(OptionAgent::agent()->getParam(
                "worldmap", "script/worldmap.lua"));
    WorldMap *worldmap = new WorldMap();
    worldmap->initMap(pathMap);
    m_manager->pushState(NULL, worldmap);

    keyBinding();
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
 * Remember playtime.
 */
    void
GameAgent::own_shutdown()
{
    OptionAgent *options = OptionAgent::agent();
    int playtime = options->getAsInt("playtime");
    playtime += SDL_GetTicks() / 1000;
    options->setPersistent("playtime", playtime);

    delete m_manager;
}

//-----------------------------------------------------------------
/**
 * Global keystrokes.
 */
    void
GameAgent::keyBinding()
{
    BaseMsg *msg;
    KeyBinder *keyBinder = InputAgent::agent()->keyBinder();

    // fullscreen
    KeyStroke fs(SDLK_F11, KMOD_NONE);
    msg = new SimpleMsg(Name::VIDEO_NAME, "fullscreen");
    keyBinder->addStroke(fs, msg);

    // log
    KeyStroke log_plus(SDLK_KP_PLUS, KMOD_RALT);
    msg = new SimpleMsg(Name::APP_NAME, "inc_loglevel");
    keyBinder->addStroke(log_plus, msg);
    KeyStroke log_minus(SDLK_KP_MINUS, KMOD_RALT);
    msg = new SimpleMsg(Name::APP_NAME, "dec_loglevel");
    keyBinder->addStroke(log_minus, msg);
}

