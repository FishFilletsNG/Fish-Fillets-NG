/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SurfaceLock.h"

#include "SDLException.h"


//-----------------------------------------------------------------
/**
 * Lock surface when necessary.
 * @throws SDLException when surface cannot be locked
 */
SurfaceLock::SurfaceLock(SDL_Surface *surface)
{
    m_surface = surface;
    if (SDL_MUSTLOCK(m_surface)) {
        if (SDL_LockSurface(m_surface) < 0) {
            throw SDLException(ExInfo("LockSurface"));
        }
    }
}
//-----------------------------------------------------------------
/**
 * Unlock surface.
 */
SurfaceLock::~SurfaceLock()
{
    if (SDL_MUSTLOCK(m_surface)) {
        SDL_UnlockSurface(m_surface);
    }
}
