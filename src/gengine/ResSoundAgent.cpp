/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ResSoundAgent.h"

//-----------------------------------------------------------------
/**
 * Free all resources.
 */
    void
ResSoundAgent::own_shutdown()
{
    removeAll();
}
//-----------------------------------------------------------------
/**
 * Load unshared sound from file.
 * @return sound or NULL
 */
    Mix_Chunk *
ResSoundAgent::loadSound(const Path &file)
{
    Mix_Chunk *chunk = Mix_LoadWAV(file.getNative().c_str());
    if (NULL == chunk) {
        LOG_WARNING(ExInfo("cannot load wav")
            .addInfo("path", file.getNative())
            .addInfo("MixError", Mix_GetError()));
    }
    return chunk;
}
//-----------------------------------------------------------------
/**
 * Store sound under this name.
 * Nothing is stored when sound cannot be loaded.
 */
    void
ResSoundAgent::addSound(const std::string &name, const Path &file)
{
    Mix_Chunk *chunk = loadSound(file);
    if (chunk) {
        addRes(name, chunk);
    }
}
//-----------------------------------------------------------------
    void
ResSoundAgent::unloadRes(Mix_Chunk *res)
{
    Mix_FreeChunk(res);
}

