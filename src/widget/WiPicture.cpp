/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WiPicture.h"

#include "ResImagePack.h"

//-----------------------------------------------------------------
WiPicture::WiPicture(const Path &picture)
{
    m_surface = ResImagePack::loadImage(picture);
}
//-----------------------------------------------------------------
WiPicture::~WiPicture()
{
    SDL_FreeSurface(m_surface);
}
//-----------------------------------------------------------------
void
WiPicture::drawOn(SDL_Surface *screen)
{
    SDL_Rect rect;
    rect.x = m_shift.getX();
    rect.y = m_shift.getY();
    SDL_BlitSurface(m_surface, NULL, screen, &rect);
}

