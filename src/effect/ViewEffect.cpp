/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ViewEffect.h"

#include "SurfaceLock.h"
#include "PixelTool.h"
#include "Random.h"

#include <assert.h>

//-----------------------------------------------------------------
/**
 * Standard effect is NONE.
 */
ViewEffect::ViewEffect()
{
    m_effect = EFFECT_NONE;
    m_disint = 0;
}
//-----------------------------------------------------------------
void
ViewEffect::setEffect(eEffect effect)
{
    m_effect = effect;
    if (m_effect == EFFECT_DISINTEGRATE) {
        m_disint = DISINT_START;
    }
}
//-----------------------------------------------------------------
void
ViewEffect::updateEffect()
{
    if (m_disint > 0) {
        m_disint -= 30;
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
ViewEffect::isDisintegrated() const
{
    return (m_effect == EFFECT_DISINTEGRATE && 0 == m_disint);
}
//-----------------------------------------------------------------
bool
ViewEffect::isInvisible() const
{
    return (m_effect == EFFECT_INVISIBLE) || isDisintegrated();
}
//-----------------------------------------------------------------
void
ViewEffect::blit(SDL_Surface *screen, SDL_Surface *surface, int x, int y)
{
    switch (m_effect) {
        case EFFECT_NONE:
            blitNone(screen, surface, x, y);
            break;
        case EFFECT_DISINTEGRATE:
            blitDisInt(screen, surface, x, y);
            break;
        case EFFECT_MIRROR:
            blitMirror(screen, surface, x, y);
            break;
        case EFFECT_INVISIBLE:
            break;
        case EFFECT_REVERSE:
            blitReverse(screen, surface, x, y);
            break;
        default:
            assert(!"unknown effect");
            break;
    }
}
//-----------------------------------------------------------------
/**
 * Blit without special effect.
 */
void
ViewEffect::blitNone(SDL_Surface *screen, SDL_Surface *surface, int x, int y)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    SDL_BlitSurface(surface, NULL, screen, &rect);
}
//-----------------------------------------------------------------
/**
 * Disintegration effect.
 * Draw only some pixels.
 */
void
ViewEffect::blitDisInt(SDL_Surface *screen, SDL_Surface *surface, int x, int y)
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
//-----------------------------------------------------------------
/**
 * Mirror effect. Draw left side inside.
 * The pixel in the middle will be used as a mask.
 * NOTE: mirror object should be draw as the last
 */
void
ViewEffect::blitMirror(SDL_Surface *screen, SDL_Surface *surface, int x, int y)
{
    SurfaceLock lock1(screen);
    SurfaceLock lock2(surface);

    SDL_Color mask = PixelTool::getColor(surface,
            surface->w / 2, surface->h / 2);

    for (int py = 0; py < surface->h; ++py) {
        for (int px = 0; px < surface->w; ++px) {
            SDL_Color pixel = PixelTool::getColor(surface, px, py);
            if (px > MIRROR_BORDER && PixelTool::colorEquals(pixel, mask)) {
                SDL_Color sample = PixelTool::getColor(screen,
                        x - px + MIRROR_BORDER, y + py);
                PixelTool::putColor(screen, x + px, y + py, sample);
            }
            else {
                if (pixel.unused == 255) {
                    PixelTool::putColor(screen, x + px, y + py, pixel);
                }
            }
        }
    }
}
//-----------------------------------------------------------------
/**
 * Reverse left and right.
 */
void
ViewEffect::blitReverse(SDL_Surface *screen, SDL_Surface *surface, int x, int y)
{
    SurfaceLock lock1(screen);
    SurfaceLock lock2(surface);

    for (int py = 0; py < surface->h; ++py) {
        for (int px = 0; px < surface->w; ++px) {
            SDL_Color pixel = PixelTool::getColor(surface, px, py);
            if (pixel.unused == 255) {
                PixelTool::putColor(screen,
                        x + surface->w - 1 - px, y + py, pixel);
            }
        }
    }
}

