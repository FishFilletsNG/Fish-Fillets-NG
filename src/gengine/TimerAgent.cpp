/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "TimerAgent.h"

//-----------------------------------------------------------------
    void
TimerAgent::own_init()
{
    m_lastTime = SDL_GetTicks();
    m_nextTime = m_lastTime;
    m_deltaTime = 1;
    m_count = 0;
}
//-----------------------------------------------------------------
/**
 * Sleep fixed number miliseconds.
 */
    void
TimerAgent::own_update()
{
    m_count++;

    Uint32 now = SDL_GetTicks();
    if (now < m_nextTime) {
        SDL_Delay(m_nextTime - now);
    }

    now = SDL_GetTicks();
    //NOTE: there are two posibilities
    m_nextTime += TIMEINTERVAL;
    //mNextTime = now + TIMEINTERVAL;

    m_deltaTime = now - m_lastTime;
    m_lastTime = now;
}
