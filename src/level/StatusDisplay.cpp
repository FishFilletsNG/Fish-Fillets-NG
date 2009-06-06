/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "StatusDisplay.h"

#include "Picture.h"

//-----------------------------------------------------------------
StatusDisplay::StatusDisplay()
{
    m_picture = NULL;
    m_time = 0;
}
//-----------------------------------------------------------------
StatusDisplay::~StatusDisplay()
{
    if (m_picture) {
        delete m_picture;
    }
}
//-----------------------------------------------------------------
/**
 * Dislay this picture the given number of times.
 */
void
StatusDisplay::displayStatus(Picture *new_picture, int time)
{
    if (m_picture) {
        delete m_picture;
    }
    m_picture = new_picture;
    m_time = time;
}
//-----------------------------------------------------------------
void
StatusDisplay::drawOn(SDL_Surface *screen)
{
    if (m_time > 0) {
        m_time--;
        if (m_picture) {
            m_picture->drawOn(screen);
        }
    }
}

