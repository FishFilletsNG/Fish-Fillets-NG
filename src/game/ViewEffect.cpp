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
#include "Random.h"

#include <assert.h>

//-----------------------------------------------------------------
/**
 * Standard effect is NONE.
 */
ViewEffect::ViewEffect()
{
    m_effect = EFFECT_NONE;
    m_disint = 255;
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
            //TODO: implement mirror effect
            blitNone(screen, surface, x, y);
            break;
        default:
            assert("unknown effect" == NULL);
    }
}
//-----------------------------------------------------------------
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
                Uint32 pixel = getPixel(surface, px, py);
                Uint8 alpha = getAlpha(pixel, surface->format);
                if (alpha == 255) {
                    putPixel(screen, x + px, y + py, pixel);
                }
            }
        }
    }
}
//-----------------------------------------------------------------
/**
 * Get Alpha component.
 */
Uint8
ViewEffect::getAlpha(Uint32 pixel, SDL_PixelFormat *fmt)
{
    Uint32 temp = pixel & fmt->Amask; // Isolate alpha component
    temp = temp >> fmt->Ashift; // Shift it down to 8-bit
    temp = temp << fmt->Aloss; // Expand to a full 8-bit number
    return temp;
}
//-----------------------------------------------------------------
/**
 * Get pixel at x, y.
 * Surface must be locked.
 * @return pixel
 */
    Uint32
ViewEffect::getPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = static_cast<Uint8*>(surface->pixels) + y * surface->pitch
        + x * bpp;

    switch(bpp) {
        case 1: // 8bit
            return *p;
        case 2: // 16bit 
            return *reinterpret_cast<Uint16*>(p);
        case 3: // 24bit 
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                return p[0] << 16 | p[1] << 8 | p[2];
            }
            else {
                return p[0] | p[1] << 8 | p[2] << 16;
            }
        case 4: // 32 bit
            return *reinterpret_cast<Uint32*>(p);
        default:
            assert("unknown color default" == NULL);
            return 0;
    }
}
//-----------------------------------------------------------------
/**
 * Put pixel at x, y.
 * Surface must be locked.
 */
    void
ViewEffect::putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    if ((0 <= x && x < surface->w) && (0 <= y && y < surface->h)) {
        int bpp = surface->format->BytesPerPixel;
        Uint8 *p = static_cast<Uint8*>(surface->pixels) + y * surface->pitch
            + x * bpp;

        switch(bpp) {
            case 1:
                *p = pixel;
                break;
            case 2:
                *reinterpret_cast<Uint16*>(p) = pixel;
                break;
            case 3:
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                    p[0] = (pixel >> 16) & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = pixel & 0xff;
                } else {
                    p[0] = pixel & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = (pixel >> 16) & 0xff;
                }
                break;
            case 4:
                *reinterpret_cast<Uint32*>(p) = pixel;
                break;
            default:
                assert("unknown color depth" == NULL);
        }
    }
}


