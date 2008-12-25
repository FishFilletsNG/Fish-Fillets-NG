/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SurfaceTool.h"

#include "SDLException.h"

//-----------------------------------------------------------------
/**
 * Return new empty surface with the same format.
 * @param surface surface to ask for pixel format
 * @param width width or 0 for the same width
 * @param height height or 0 for the same height
 * @throws SDLException when function fails
 */
    SDL_Surface *
SurfaceTool::createEmpty(SDL_Surface *surface, int width, int height)
{
    if (!width) {
        width = surface->w;
    }
    if (!height) {
        height = surface->h;
    }

    SDL_Surface *result = SDL_CreateRGBSurface(surface->flags, width, height,
            surface->format->BitsPerPixel,
            surface->format->Rmask,
            surface->format->Gmask,
            surface->format->Bmask,
            surface->format->Amask);
    if (NULL == result) {
        throw SDLException(ExInfo("CreateRGBSurface"));
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Return new surface with transparent background.
 */
    SDL_Surface *
SurfaceTool::createTransparent(int w, int h, const SDL_Color &transparent)
{
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCCOLORKEY,
            w, h, 32,
            0, 0, 0, 0);
    if (NULL == surface) {
        throw SDLException(ExInfo("CreateRGBSurface"));
    }

    Uint32 transparentKey = SDL_MapRGB(surface->format,
            transparent.r, transparent.g, transparent.b);
    SDL_SetColorKey(surface, SDL_SRCCOLORKEY|SDL_RLEACCEL, transparentKey);

    SurfaceTool::alphaFill(surface, NULL, transparent);
    return surface;
}
//-----------------------------------------------------------------
/**
 * Return new cloned surface.
 * @return new surface, free it after use
 * @throws SDLException when function fails
 */
    SDL_Surface *
SurfaceTool::createClone(SDL_Surface *surface)
{
    SDL_Surface *clone = SDL_ConvertSurface(surface,
            surface->format, surface->flags);
    if (NULL == clone) {
        throw SDLException(ExInfo("ConvertSurface"));
    }
    return clone;
}
//-----------------------------------------------------------------
/**
 * Fill surface with given color. Alpha value in color is supported.
 * The final blit rectangle is saved in dstrect.
 *
 * @param surface surface to fill
 * @param dstrect dstrect or NULL (the whole surface will be filled with color).
 * @param color {red, green, blue, alpha}
 */
    void
SurfaceTool::alphaFill(SDL_Surface *surface, SDL_Rect *dstrect,
        const SDL_Color &color)
{
    int w = surface->w;
    int h = surface->h;
    if (dstrect) {
        w = dstrect->w;
        h = dstrect->h;
    }
    SDL_Surface *canvas = createEmpty(surface, w, h);
    Uint32 pixel = SDL_MapRGB(canvas->format, color.r, color.g, color.b);
    SDL_FillRect(canvas, NULL, pixel);
    SDL_SetAlpha(canvas, SDL_SRCALPHA|SDL_RLEACCEL, color.unused);

    SDL_BlitSurface(canvas, NULL, surface, dstrect);
    SDL_FreeSurface(canvas);
}

