/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LayeredPicture.h"

#include "Path.h"
#include "ResImagePack.h"
#include "ResourceException.h"
#include "SurfaceLock.h"
#include "PixelTool.h"

//-----------------------------------------------------------------
/**
 * Create picture with two layers and color mask to select
 * active areas.
 *
 * @throws ResourceException when lowerLayer and colorMask have
 * different proportions
 */
LayeredPicture::LayeredPicture(const Path &bg_file, const V2 &loc,
        const Path &lowerLayer, const Path &colorMask)
: Picture(bg_file, loc)
{
    m_lowerLayer = ResImagePack::loadImage(lowerLayer);
    m_colorMask = ResImagePack::loadImage(colorMask);
    if (m_lowerLayer->w != m_colorMask->w
            || m_lowerLayer->h != m_colorMask->h) {
        SDL_FreeSurface(m_lowerLayer);
        SDL_FreeSurface(m_colorMask);
        SDL_FreeSurface(m_surface);

        throw ResourceException(ExInfo(
                    "lowerLayer and colorMask have different proportions")
                .addInfo("lowerLayer", lowerLayer.getNative())
                .addInfo("colorMask", colorMask.getNative()));
    }

    setNoActive();
}
//-----------------------------------------------------------------
LayeredPicture::~LayeredPicture()
{
    SDL_FreeSurface(m_lowerLayer);
    SDL_FreeSurface(m_colorMask);
}
//-----------------------------------------------------------------
/**
 * Return pixel at worldLoc.
 * Translates world coordinates to local coordinates.
 */
    Uint32
LayeredPicture::getMaskAtWorld(const V2 &worldLoc)
{
    V2 localLoc = worldLoc.minus(m_loc);
    return getMaskAt(localLoc);
}
//-----------------------------------------------------------------
/**
 * Return pixel at position from left top image corner.
 */
    Uint32
LayeredPicture::getMaskAt(const V2 &loc)
{
    Uint32 result = MASK_NO;

    if ((0 <= loc.getX() && loc.getX() < m_colorMask->w)
            && (0 <= loc.getY() && loc.getY() < m_colorMask->h))
    {
        SurfaceLock lock1(m_colorMask);
        result = PixelTool::getPixel(m_colorMask,
                loc.getX(), loc.getY());
    }
    return result;
}
//-----------------------------------------------------------------
    void
LayeredPicture::drawOn(SDL_Surface *screen)
{
    Picture::drawOn(screen);
    if (m_activeColor == MASK_NO) {
        return;
    }

    SurfaceLock lock1(screen);
    SurfaceLock lock2(m_lowerLayer);
    SurfaceLock lock3(m_colorMask);

    //TODO: support alpha channels
    for (int py = 0; py < m_colorMask->h; ++py) {
        int world_y = m_loc.getY() + py;
        for (int px = 0; px < m_colorMask->w; ++px) {
            Uint32 sample = PixelTool::getPixel(m_colorMask, px, py);

            if (sample == m_activeColor) {
                SDL_Color lower = PixelTool::getColor(m_lowerLayer, px, py);
                if (lower.unused == 255) {
                    PixelTool::putColor(screen,
                            m_loc.getX() + px, world_y, lower);
                }
            }
        }
    }
}

