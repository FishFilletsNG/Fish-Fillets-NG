/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ResSoundPack.h"

#include "Path.h"
#include "OptionAgent.h"

//-----------------------------------------------------------------
    void
ResSoundPack::unloadRes(Mix_Chunk *res)
{
    Mix_FreeChunk(res);
}
//-----------------------------------------------------------------
/**
 * Load unshared sound from file.
 * @return sound or NULL
 */
    Mix_Chunk *
ResSoundPack::loadSound(const Path &file)
{
    Mix_Chunk *chunk = NULL;
    //TODO: ask SoundAgent to load this sound
    if (OptionAgent::agent()->getAsBool("sound", true)) {
        chunk = Mix_LoadWAV(file.getNative().c_str());
        if (NULL == chunk) {
            LOG_WARNING(ExInfo("cannot load sound")
                .addInfo("path", file.getNative())
                .addInfo("MixError", Mix_GetError()));
        }
    }
    return chunk;
}
//-----------------------------------------------------------------
/**
 * Store sound under this name.
 * Nothing is stored when sound cannot be loaded.
 */
    void
ResSoundPack::addSound(const std::string &name, const Path &file)
{
    Mix_Chunk *chunk = loadSound(file);
    if (chunk) {
        addRes(name, chunk);
    }
}

