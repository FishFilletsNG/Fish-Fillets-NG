/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "EffectZx.h"

#include "SurfaceLock.h"
#include "PixelTool.h"
#include "Random.h"

//-----------------------------------------------------------------
/**
 * Read colors from all four corners.
 */
EffectZx::EffectZx()
{
    m_zx = ZX1;
    m_phase = 0;
    m_countHeight = 0;
    m_stripeHeight = STRIPE_STANDARD;
}
//-----------------------------------------------------------------
/**
 * Update sprite height as ZX Spectrum does.
 */
    void
EffectZx::updateEffect()
{
    m_phase = (m_phase + 1) % 500;
    if (m_phase == 1) {
        m_zx = ZX1;
        m_stripeHeight = STRIPE_STANDARD;
    }
    else if (2 <= m_phase && m_phase <= 51) {
        m_stripeHeight = (m_stripeHeight * 3
                * (0.97 + Random::randomReal(0.06))
                + STRIPE_STANDARD) / 4.0;
    }
    else if (m_phase == 52) {
        m_zx = ZX3;
        m_stripeHeight = STRIPE_NARROW;
    }
    else {
        m_stripeHeight = (m_stripeHeight * 3
                * (0.95 + Random::randomReal(0.1))
                + STRIPE_NARROW) / 4.0;
    }
}
//-----------------------------------------------------------------
/**
 * Draw ZX spectrum loading.
 */
    void
EffectZx::blit(SDL_Surface *screen, SDL_Surface *surface, int x, int y)
{
    SurfaceLock lock1(screen);
    SurfaceLock lock2(surface);

    SDL_Color colorZX1 = PixelTool::getColor(surface, 0, 0);
    SDL_Color colorZX2 = PixelTool::getColor(surface, 0, surface->h - 1);
    SDL_Color colorZX3 = PixelTool::getColor(surface, surface->w - 1, 0);
    SDL_Color colorZX4 = PixelTool::getColor(surface,
            surface->w - 1, surface->h - 1);

    for (int py = 0; py < surface->h; ++py) {
        m_countHeight++;
        if (m_countHeight > m_stripeHeight) {
            m_countHeight -= m_stripeHeight;
            switch (m_zx) {
                case ZX1:
                    m_zx = ZX2;
                    break;
                case ZX2:
                    m_zx = ZX1;
                    break;
                case ZX3:
                    m_zx = ZX4;
                    break;
                default:
                    m_zx = ZX3;
                    break;
            }
        }

        SDL_Color *usedColor;
        switch (m_zx) {
            case ZX1:
                usedColor = &colorZX1;
                break;
            case ZX2:
                usedColor = &colorZX2;
                break;
            case ZX3:
                usedColor = &colorZX3;
                break;
            default:
                usedColor = &colorZX4;
                break;
        }

        for (int px = 0; px < surface->w; ++px) {
            SDL_Color pixel = PixelTool::getColor(surface, px, py);
            if (pixel.unused == 255) {
                PixelTool::putColor(screen,
                        x + px, y + py, *usedColor);
            }
        }
    }
}

