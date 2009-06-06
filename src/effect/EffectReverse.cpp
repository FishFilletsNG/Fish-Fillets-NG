/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "EffectReverse.h"

#include "SurfaceLock.h"
#include "PixelTool.h"

const char *EffectReverse::NAME = "reverse";
//-----------------------------------------------------------------
/**
 * Reverse left and right.
 */
void
EffectReverse::blit(SDL_Surface *screen, SDL_Surface *surface, int x, int y)
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

