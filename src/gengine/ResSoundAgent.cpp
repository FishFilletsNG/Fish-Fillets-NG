/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ResSoundAgent.h"

#include "ResSoundPack.h"

//-----------------------------------------------------------------
    void
ResSoundAgent::own_init()
{
    m_pack = new ResSoundPack();
}
//-----------------------------------------------------------------
/**
 * Free all resources.
 */
    void
ResSoundAgent::own_shutdown()
{
    m_pack->removeAll();
    delete m_pack;
}
//-----------------------------------------------------------------
/**
 * Load unshared sound from file.
 * @return sound or NULL
 */
    Mix_Chunk *
ResSoundAgent::loadSound(const Path &file)
{
    return m_pack->loadSound(file);
}

