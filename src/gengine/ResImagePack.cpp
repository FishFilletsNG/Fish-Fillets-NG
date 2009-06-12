/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ResImagePack.h"

#include "Path.h"
#include "ImgException.h"
#include "SDLException.h"
#include "OptionAgent.h"
#include "Log.h"

#include "SDL_image.h"

// The set cache size allows to contain all fish images and animations
// from level 'barrel'.
ResCache<SDL_Surface*> *ResImagePack::CACHE = new ResCache<SDL_Surface*>(
        265, new ResImagePack(false));

//-----------------------------------------------------------------
ResImagePack::ResImagePack(bool caching_enabled) {
    m_caching_enabled = false;
    if (caching_enabled) {
        m_caching_enabled = OptionAgent::agent()->getAsBool(
                "cache_images", true);
    }
}
//-----------------------------------------------------------------
/**
 * Load unshared image from file
 * and convert image to diplayformat.
 *
 * @return loaded surface
 * @throws ImgException when image cannot be loaded
 * @throws SDLException when image cannot be converted
 */
SDL_Surface *
ResImagePack::loadImage(const Path &file)
{
    SDL_Surface *raw_image = IMG_Load(file.getNative().c_str());
    if (NULL == raw_image) {
        throw ImgException(ExInfo("Load")
                .addInfo("file", file.getNative()));
    }

    SDL_Surface *surface = SDL_DisplayFormatAlpha(raw_image);
    if (NULL == surface) {
        throw SDLException(ExInfo("DisplayFormat")
                .addInfo("file", file.getNative()));
    }
    SDL_FreeSurface(raw_image);

    return surface;
}
//-----------------------------------------------------------------
/**
 * Store image from file.
 *
 * @throws ImgException when image cannot be loaded
 * @throws SDLException when image cannot be converted
 */
void
ResImagePack::addImage(const std::string &name, const Path &file)
{
    SDL_Surface *surface;
    if (m_caching_enabled) {
        surface = CACHE->get(file.getPosixName());
        if (!surface) {
            surface = loadImage(file);
            CACHE->put(file.getPosixName(), surface);
        }
    } else {
        surface = loadImage(file);
    }

    addRes(name, surface);
}
//-----------------------------------------------------------------
void
ResImagePack::unloadRes(SDL_Surface *res)
{
    if (m_caching_enabled) {
        CACHE->release(res);
    } else {
        SDL_FreeSurface(res);
    }
}

