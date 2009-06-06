/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "EffectNone.h"

const char *EffectNone::NAME = "none";
//-----------------------------------------------------------------
/**
 * Blit without special effect.
 */
void
EffectNone::blit(SDL_Surface *screen, SDL_Surface *surface, int x, int y)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    SDL_BlitSurface(surface, NULL, screen, &rect);
}

