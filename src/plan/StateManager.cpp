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
    emptyTrash();

    t_states::iterator end = m_states.end();
    for (t_states::iterator i = m_states.begin(); i != end; ++i) {
        delete (*i);
    }
}
//-----------------------------------------------------------------
    void
StateManager::emptyTrash()
{
    t_states::iterator end = m_trash.end();
    for (t_states::iterator i = m_trash.begin(); i != end; ++i) {
        delete (*i);
    }
    m_trash.clear();
}
//-----------------------------------------------------------------
/**
 * Update current state and states on background
 * and empty trash.
 * The states at top will be updated as first.
 */
void
StateManager::updateGame()
{
    t_states::reverse_iterator rend = m_states.rend();
    for (t_states::reverse_iterator i = m_states.rbegin();
            i != rend && (*i)->isRunning(); ++i)
    {
        (*i)->updateState();
    }

    emptyTrash();
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
StateManager::changeState(GameState *new_state)
{
    removeCurrent();
    if (!m_states.empty()) {
        if (new_state->allowBg()) {
            resumeStack();
            m_states.back()->noteBg();
        }
        else {
            pauseRunning();
        }
    }

    new_state->initState(this);
    m_states.push_back(new_state);
}
//-----------------------------------------------------------------
/**
 * Pause current state and activate this one.
 */
void
StateManager::pushState(GameState *new_state)
{
    if (!m_states.empty()) {
        if (new_state->allowBg()) {
            m_states.back()->noteBg();
        }
        else {
            pauseRunning();
        }
    }

    new_state->initState(this);
    m_states.push_back(new_state);
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
        resumeStack();
    }
    else {
        MessagerAgent::agent()->forwardNewMsg(
                new SimpleMsg(Name::APP_NAME, "quit"));
    }
}

//-----------------------------------------------------------------
/**
 * Resume stack after change at top.
 */
void
StateManager::resumeStack()
{
    if (!m_states.empty()) {
        if (m_states.back()->isRunning()) {
            m_states.back()->noteFg();
        }
        else {
            resumeBg(m_states.end() - 1);
        }
    }
}
//-----------------------------------------------------------------
/**
 * Recursive resume all states on stack which is allowed.
 * The lowers will be resumed first.
 */
void
StateManager::resumeBg(t_states::iterator state)
{
    if ((*state)->allowBg() && state != m_states.begin()) {
        t_states::iterator bg = state - 1;
        resumeBg(bg);
        (*bg)->noteBg();
    }
    (*state)->resumeState();
}
//-----------------------------------------------------------------
/**
 * Pause all running states.
 * The states at top will be paused as first.
 */
void
StateManager::pauseRunning()
{
    t_states::reverse_iterator rend = m_states.rend();
    for (t_states::reverse_iterator i = m_states.rbegin();
            i != rend && (*i)->isRunning(); ++i)
    {
        (*i)->pauseState();
    }
}


