/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SDLException.h"

#include "SDL.h"

//-----------------------------------------------------------------
/**
 * Add SDL_GetError to info.
 * @param problem detail info
 */
SDLException::SDLException(const ExInfo &problem)
    : BaseException(ExInfo(problem).addInfo("SDL", SDL_GetError()))
{
    /* empty */
}
