/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "StateManager.h"

#include "GameState.h"

#include "MessagerAgent.h"
#include "SimpleMsg.h"

//-----------------------------------------------------------------
StateManager::~StateManager()
{
    t_states::iterator end = m_states.end();
    for (t_states::iterator i = m_states.begin(); i != end; ++i) {
        delete (*i);
    }
}
//-----------------------------------------------------------------
/**
 * Update current state
 * and empty trash.
 */
void
StateManager::updateGame()
{
    if (!m_states.empty()) {
        m_states.back()->updateState();
    }

    t_states::iterator end = m_trash.end();
    for (t_states::iterator i = m_trash.begin(); i != end; ++i) {
        delete (*i);
    }
    m_trash.clear();
}

//-----------------------------------------------------------------
/**
 * Throw current state to the trash.
 * NOTE: current state is still active, cannot be deleted
 */
void
StateManager::removeCurrent()
{
    if (!m_states.empty()) {
        m_states.back()->cleanState();
        m_trash.push_back(m_states.back());
        m_states.pop_back();
    }
}
//-----------------------------------------------------------------
/**
 * Remove current state and set this one.
 */
void
StateManager::changeState(GameState *newState)
{
    removeCurrent();

    newState->initState(this);
    m_states.push_back(newState);
}
//-----------------------------------------------------------------
/**
 * Pause current state and set this one.
 */
void
StateManager::pushState(GameState *newState)
{
    if (!m_states.empty()) {
        m_states.back()->pauseState();
    }

    newState->initState(this);
    m_states.push_back(newState);
}
//-----------------------------------------------------------------
/**
 * Remove current state and resume paused state.
 */
void
StateManager::popState()
{
    removeCurrent();

    if (!m_states.empty()) {
        m_states.back()->resumeState();
    }
    else {
        MessagerAgent::agent()->forwardNewMsg(
                new SimpleMsg(Name::APP_NAME, "quit"));
    }
}

