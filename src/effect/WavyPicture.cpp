/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WavyPicture.h"

#include "TimerAgent.h"

#include <math.h>

//-----------------------------------------------------------------
/**
 * Load surface.
 * Default is no waves.
 */
WavyPicture::WavyPicture(const Path &file, const V2 &loc)
    : Picture(file, loc)
{
    m_amp = 0;
    m_periode = m_surface->w;
    m_speed = 0;
}
//-----------------------------------------------------------------
/**
 * Blit entire surface to [x,y].
 * Do vertical waves with phase shift.
 */
void
WavyPicture::drawOn(SDL_Surface *screen)
{
    if (m_amp == 0) {
        Picture::drawOn(screen);
        return;
    }

    //NOTE: Wamp = Wamp_in_orig/2.0
    //NOTE: Wspeed = 1.0/Wspd_in_orig
    SDL_Rect dest_rect;
    SDL_Rect line_rect;
    line_rect.w = m_surface->w;
    line_rect.h = 1;

    double shift = TimerAgent::agent()->getCycles() * m_speed;

    for (int py = 0; py < m_surface->h; ++py) {
        dest_rect.x = m_loc.getX();
        dest_rect.y = m_loc.getY() + py;

        double wave = m_amp * sin(py / m_periode + shift);
        line_rect.x = static_cast<int>(0.5 + wave);
        line_rect.y = py;

        SDL_BlitSurface(m_surface, &line_rect, screen, &dest_rect);
    }
}


