/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ResFontPack.h"

#include "ResourceException.h"
#include "ResImagePack.h"

//-----------------------------------------------------------------
/**
 * Load font.
 * @throws ResourceException when font cannot be loaded
 */
SFont_Font *
ResFontPack::loadFont(const Path &file)
{
    SDL_Surface *font_face = ResImagePack::loadImage(file);

    SFont_Font *font = SFont_InitFont(font_face);
    if (NULL == font) {
        throw ResourceException(ExInfo("cannot create font")
                .addInfo("font", file.getNative()));
    }

    return font;
}
//-----------------------------------------------------------------
/**
 * Store font from file.
 */
void
ResFontPack::addFont(const std::string &name, const Path &file)
{
    SFont_Font *font = loadFont(file);
    addRes(name, font);
}
//-----------------------------------------------------------------
void
ResFontPack::unloadRes(SFont_Font *res)
{
    SFont_FreeFont(res);
}

