/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "GameState.h"

#include "StateManager.h"
#include "InputHandler.h"

#include "Log.h"
#include "LogicException.h"
#include "InputAgent.h"

//-----------------------------------------------------------------
GameState::GameState()
{
    m_handler = NULL;
}
//-----------------------------------------------------------------
GameState::~GameState()
{
    if (m_handler) {
        delete m_handler;
    }
}
//-----------------------------------------------------------------
/**
 * Obtain input handler.
 * @param handler new input handler
 */
void
GameState::takeHandler(InputHandler *new_handler)
{
    if (m_handler) {
        delete m_handler;
    }
    m_handler = new_handler;
}
//-----------------------------------------------------------------
void
GameState::initState(StateManager *manager)
{
    LOG_INFO(ExInfo("initState").addInfo("name", getName()));
    m_manager = manager;
    m_active = true;
    InputAgent::agent()->installHandler(m_handler);
    own_initState();
}
//-----------------------------------------------------------------
/**
 * @throws LogicException when state is not active
 */
void
GameState::updateState()
{
    if (!m_active) {
        throw LogicException(ExInfo("state is not active")
            .addInfo("name", getName()));
    }

    own_updateState();
}
//-----------------------------------------------------------------
/**
 * @throws LogicException when state is not active
 */
void
GameState::pauseState()
{
    LOG_INFO(ExInfo("pauseState").addInfo("name", getName()));
    if (!m_active) {
        throw LogicException(ExInfo("pause - state is not active")
            .addInfo("name", getName()));
    }

    own_pauseState();
    InputAgent::agent()->installHandler(NULL);
    m_active = false;
}
//-----------------------------------------------------------------
/**
 * @throws LogicException when state is not active
 */
void
GameState::resumeState()
{
    LOG_INFO(ExInfo("resumeState").addInfo("name", getName()));
    m_active = true;
    InputAgent::agent()->installHandler(m_handler);
    own_resumeState();
}
//-----------------------------------------------------------------
void
GameState::cleanState()
{
    LOG_INFO(ExInfo("cleanState").addInfo("name", getName()));
    own_cleanState();

    InputAgent::agent()->installHandler(NULL);
    m_active = false;
    m_manager = NULL;
}
//-----------------------------------------------------------------
void
GameState::quitState()
{
    m_manager->popState();
}


