/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LangFlag.h"

#include "ResImagePack.h"
#include "V2.h"

//-----------------------------------------------------------------
LangFlag::LangFlag(const std::string &lang, const Path &picture)
    : m_lang(lang)
{
    m_surface = ResImagePack::loadImage(picture);
}
//-----------------------------------------------------------------
LangFlag::~LangFlag()
{
    SDL_FreeSurface(m_surface);
}
//-----------------------------------------------------------------
void
LangFlag::drawOn(SDL_Surface *screen, const V2 &loc)
{
    SDL_Rect rect;
    rect.x = loc.getX();
    rect.y = loc.getY();
    SDL_BlitSurface(m_surface, NULL, screen, &rect);
}

