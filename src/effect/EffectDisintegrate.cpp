/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "EffectDisintegrate.h"

#include "SurfaceLock.h"
#include "PixelTool.h"
#include "Random.h"

const char *EffectDisintegrate::NAME = "disintegrate";
//-----------------------------------------------------------------
/**
 * Start as not disintegrated.
 */
EffectDisintegrate::EffectDisintegrate()
{
    m_disint = DISINT_START;
}
//-----------------------------------------------------------------
void
EffectDisintegrate::updateEffect()
{
    if (m_disint > 0) {
        m_disint -= DISINT_SPEED;
        if (m_disint < 0) {
            m_disint = 0;
        }
    }
}
//-----------------------------------------------------------------
/**
 * Returns true for object for who the disint effect is finished.
 */
bool
EffectDisintegrate::isDisintegrated() const
{
    return 0 == m_disint;
}
//-----------------------------------------------------------------
bool
EffectDisintegrate::isInvisible() const
{
    return isDisintegrated();
}
//-----------------------------------------------------------------
/**
 * Disintegration effect.
 * Draw only some pixels.
 */
void
EffectDisintegrate::blit(SDL_Surface *screen, SDL_Surface *surface,
        int x, int y)
{
    SurfaceLock lock1(screen);
    SurfaceLock lock2(surface);

    for (int py = 0; py < surface->h; ++py) {
        for (int px = 0; px < surface->w; ++px) {
            if (Random::aByte(py * surface->w + px) < m_disint) {
                SDL_Color pixel = PixelTool::getColor(surface, px, py);
                if (pixel.unused == 255) {
                    PixelTool::putColor(screen, x + px, y + py, pixel);
                }
            }
        }
    }
}

