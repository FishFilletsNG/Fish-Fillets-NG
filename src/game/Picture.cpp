/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Picture.h"


#include "ResImageAgent.h"

//-----------------------------------------------------------------
/**
 * Load surface.
 */
Picture::Picture(const Path &file, int x, int y)
{
    m_x = x;
    m_y = y;

    m_surface = ResImageAgent::agent()->loadImage(file);
}
//-----------------------------------------------------------------
/**
 * Free surface.
 */
Picture::~Picture()
{
    SDL_FreeSurface(m_surface);
}
//-----------------------------------------------------------------
/**
 * Blit entire surface to [x,y].
 */
void
Picture::draw()
{
    SDL_Rect rect;
    rect.x = m_x;
    rect.y = m_y;

    SDL_BlitSurface(m_surface, NULL, m_screen, &rect);
}


