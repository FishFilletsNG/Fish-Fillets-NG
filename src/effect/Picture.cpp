/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Picture.h"

#include "ResImagePack.h"

//-----------------------------------------------------------------
/**
 * Load surface.
 */
Picture::Picture(const Path &file, const V2 &loc)
    : m_loc(loc)
{
    m_surface = ResImagePack::loadImage(file);
}
//-----------------------------------------------------------------
/**
 * Use this surface.
 */
Picture::Picture(SDL_Surface *new_surface, const V2 &loc)
    : m_loc(loc)
{
    m_surface = new_surface;
}

//-----------------------------------------------------------------
/**
 * Free surface.
 */
Picture::~Picture()
{
    SDL_FreeSurface(m_surface);
}
//-----------------------------------------------------------------
void
Picture::changePicture(const Path &file)
{
    SDL_FreeSurface(m_surface);
    m_surface = ResImagePack::loadImage(file);
}
//-----------------------------------------------------------------
void
Picture::changePicture(SDL_Surface *new_surface)
{
    SDL_FreeSurface(m_surface);
    m_surface = new_surface;
}
//-----------------------------------------------------------------
/**
 * Blit entire surface to [x,y].
 */
void
Picture::drawOn(SDL_Surface *screen)
{
    SDL_Rect rect;
    rect.x = m_loc.getX();
    rect.y = m_loc.getY();

    SDL_BlitSurface(m_surface, NULL, screen, &rect);
}


