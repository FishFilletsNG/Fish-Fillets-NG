/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "TTFException.h"

#include "SDL_ttf.h"

//-----------------------------------------------------------------
/**
 * Add TTF_GetError to info.
 * @param problem detail info
 */
TTFException::TTFException(const ExInfo &problem)
    : BaseException(ExInfo(problem).addInfo("TTF", TTF_GetError()))
{
    /* empty */
}
