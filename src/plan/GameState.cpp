/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "GameState.h"

#include "Log.h"
#include "LogicException.h"
#include "StateManager.h"

//-----------------------------------------------------------------
void
GameState::initState(StateManager *manager)
{
    LOG_INFO(ExInfo("initState").addInfo("name", getName()));
    m_manager = manager;
    m_active = true;
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
    own_resumeState();
}
//-----------------------------------------------------------------
void
GameState::cleanState()
{
    LOG_INFO(ExInfo("cleanState").addInfo("name", getName()));
    own_cleanState();

    m_active = false;
    m_manager = NULL;
}
//-----------------------------------------------------------------
void
GameState::quitState()
{
    m_manager->popState();
}


