/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "PixelIterator.h"

#include "SurfaceLock.h"

#include <assert.h>

//-----------------------------------------------------------------
/**
 * Creates new pixel iterator and locks the surface.
 */
PixelIterator::PixelIterator(SDL_Surface *surface)
{
    m_surface = surface;
    m_lock = new SurfaceLock(m_surface);
    m_p = static_cast<Uint8*>(surface->pixels);
    m_end = m_p + m_surface->h * m_surface->pitch;
    m_bpp = surface->format->BytesPerPixel;
}
//-----------------------------------------------------------------
/**
 * Unlocks surface.
 */
PixelIterator::~PixelIterator()
{
    delete m_lock;
}
//-----------------------------------------------------------------
    void
PixelIterator::setPos(const V2 &pos)
{
    int x = pos.getX();
    int y = pos.getY();
    assert((0 <= x && x < m_surface->w) && (0 <= y && y < m_surface->h));

    m_p = static_cast<Uint8*>(m_surface->pixels) +
        y * m_surface->pitch +
        x * m_bpp;
}
//-----------------------------------------------------------------
/**
 * Increments iterator position.
 * @return true when there is more data
 */
    bool
PixelIterator::inc()
{
    m_p += m_bpp;
    return m_p < m_end;
}
//-----------------------------------------------------------------
bool
PixelIterator::isTransparent() const
{
    return getPixel() == m_surface->format->colorkey;
}
//-----------------------------------------------------------------
SDL_Color
PixelIterator::getColor() const
{
    SDL_Color color;
    SDL_GetRGBA(getPixel(), m_surface->format,
            &color.r, &color.g, &color.b, &color.unused);
    return color;
}
//-----------------------------------------------------------------
Uint32
PixelIterator::getPixel() const
{
    switch(m_bpp) {
        case 1: // 8bit
            return *m_p;
        case 2: // 16bit 
            return *reinterpret_cast<Uint16*>(m_p);
        case 3: // 24bit 
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                return m_p[0] << 16 | m_p[1] << 8 | m_p[2];
            }
            else {
                return m_p[0] | m_p[1] << 8 | m_p[2] << 16;
            }
        case 4: // 32 bit
            return *reinterpret_cast<Uint32*>(m_p);
        default:
            assert(!"unknown color depth");
            return 0;
    }
}
//-----------------------------------------------------------------
    void
PixelIterator::putColor(const SDL_Color &color)
{
    Uint32 pixel = SDL_MapRGBA(m_surface->format,
            color.r, color.g, color.b, color.unused);
    putPixel(pixel);
}
//-----------------------------------------------------------------
    void
PixelIterator::putPixel(Uint32 pixel)
{
    switch(m_bpp) {
        case 1:
            *m_p = pixel;
            break;
        case 2:
            *reinterpret_cast<Uint16*>(m_p) = pixel;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                m_p[0] = (pixel >> 16) & 0xff;
                m_p[1] = (pixel >> 8) & 0xff;
                m_p[2] = pixel & 0xff;
            } else {
                m_p[0] = pixel & 0xff;
                m_p[1] = (pixel >> 8) & 0xff;
                m_p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4:
            *reinterpret_cast<Uint32*>(m_p) = pixel;
            break;
        default:
            assert(!"unknown color depth");
            break;
    }
}

