/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "PosterScroller.h"

#include "Picture.h"
#include "OptionAgent.h"
#include "minmax.h"

//-----------------------------------------------------------------
PosterScroller::PosterScroller(const Path &picture)
    : PosterState(picture)
{
    m_shift = 0;
    m_screenH = 0;
}
//-----------------------------------------------------------------
void
PosterScroller::own_initState()
{
    m_screenH = OptionAgent::agent()->getAsInt("screen_height");
    m_shift = -m_screenH + SHIFT_SPEED;
}
//-----------------------------------------------------------------
void
PosterScroller::own_updateState()
{
    int maxShift = min(m_shift, m_bg->getH() - m_screenH/3);

    m_bg->setLoc(V2(0, -maxShift));
    m_shift += SHIFT_SPEED;
    if (m_shift > m_bg->getH()) {
        quitState();
    }
}

