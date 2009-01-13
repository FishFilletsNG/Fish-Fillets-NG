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
#include "Dialog.h"

#include "SDL_image.h"

//-----------------------------------------------------------------
/**
 * Return path to a localized image or the original path.
 * The localized path has format: <name>_<lang>.<extension>
 *
 * @param original original path
 * @return localized path if it exists or the original path
 */
Path
ResImagePack::localizedPath(const Path &original)
{
    std::string path = original.getPosixName();
    std::string::size_type dotPos = path.rfind('.');
    if (dotPos == std::string::npos) {
        return original;
    }

    std::string lang = OptionAgent::agent()->getParam("lang");
    if (Dialog::DEFAULT_LANG == lang || lang.empty()) {
        return original;
    }

    std::string appendix = "_" + lang;

    std::string::size_type minDotPos = 1 + appendix.size();
    std::string::size_type dirPos = path.rfind('/');
    if (dirPos != std::string::npos) {
        minDotPos += dirPos;
    }

    if (dotPos < minDotPos) {
        return original;
    }

    path.insert(dotPos, appendix);
    Path localized = Path::dataReadPath(path);
    if (localized.exists()) {
        return localized;
    } else {
        return original;
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
    Path localized = localizedPath(file);
    SDL_Surface *raw_image = IMG_Load(localized.getNative().c_str());
    if (NULL == raw_image) {
        throw ImgException(ExInfo("Load")
                .addInfo("file", localized.getNative()));
    }

    SDL_Surface *surface = SDL_DisplayFormatAlpha(raw_image);
    if (NULL == surface) {
        throw SDLException(ExInfo("DisplayFormat")
                .addInfo("file", localized.getNative()));
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
    SDL_Surface *surface = loadImage(file);
    addRes(name, surface);
}
//-----------------------------------------------------------------
void
ResImagePack::unloadRes(SDL_Surface *res)
{
    SDL_FreeSurface(res);
}

