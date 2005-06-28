/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Outline.h"

#include "Log.h"
#include "PixelTool.h"
#include "SurfaceTool.h"
#include "SurfaceLock.h"

//-----------------------------------------------------------------
Outline::Outline(const SDL_Color &color, int width)
    : m_color(color)
{
    m_width = width;
    m_pixel = 0;
}
//-----------------------------------------------------------------
/**
 * Draw outline, use surface colorkey as bg color.
 */
void
Outline::drawOnColorKey(SDL_Surface *surface)
{
    Uint32 bgKey = surface->format->colorkey;
    drawOn(surface, bgKey);
}
//-----------------------------------------------------------------
/**
 * Draw outline on bg color.
 * @param surface picture with shape to outline
 * @param bgKey color used for background
 */
void
Outline::drawOn(SDL_Surface *surface, Uint32 bgKey)
{
    SurfaceLock lock1(surface);

    precomputePixel(surface->format);
    for (int i = 0; i < m_width; ++i) {
        drawOneLayer(surface, bgKey);
    }
}
//-----------------------------------------------------------------
void
Outline::precomputePixel(SDL_PixelFormat *format)
{
    m_pixel = SDL_MapRGB(format, m_color.r, m_color.g, m_color.b);
}
//-----------------------------------------------------------------
/**
 * Draw outline with width=1.
 */
void
Outline::drawOneLayer(SDL_Surface *surface, Uint32 bgKey)
{
    SDL_Surface *copy = SurfaceTool::createClone(surface);
    drawAlongCopy(surface, bgKey, copy);
    SDL_FreeSurface(copy);
}
//-----------------------------------------------------------------
/**
 * Draw outline with width=1.
 * Cloned surface will be used as a model. It will not be modified.
 * Destination surface must be locked.
 */
void
Outline::drawAlongCopy(SDL_Surface *surface, Uint32 bgKey, SDL_Surface *copy)
{
    SurfaceLock lock1(copy);

    for (int py = 0; py < surface->h; ++py) {
        for (int px = 0; px < surface->w; ++px) {
            if (PixelTool::getPixel(copy, px, py) != bgKey) {
                fillNeighbourhood(surface, bgKey, px, py);
            }
        }
    }
}
//-----------------------------------------------------------------
/**
 * Fill neighboors around this point.
 * Neighbours with bgKey will be filled.
 * Surface must be locked.
 * m_pixel must be precomputed.
 */
void
Outline::fillNeighbourhood(SDL_Surface *surface, Uint32 bgKey, int x, int y)
{
    //TODO: optimize for speed
    if (x > 0 && PixelTool::getPixel(surface, x - 1, y) == bgKey) {
        PixelTool::putPixel(surface, x - 1, y, m_pixel);
    }
    if (y > 0 && PixelTool::getPixel(surface, x, y - 1) == bgKey) {
        PixelTool::putPixel(surface, x, y - 1, m_pixel);
    }

    if (x + 1 < surface->w &&
            PixelTool::getPixel(surface, x + 1, y) == bgKey)
    {
        PixelTool::putPixel(surface, x + 1, y, m_pixel);
    }
    if (y + 1 < surface->h &&
            PixelTool::getPixel(surface, x, y + 1) == bgKey)
    {
        PixelTool::putPixel(surface, x, y + 1, m_pixel);
    }
}


