/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WorldInput.h"

#include "Keymap.h"

#include "KeyStroke.h"
#include "WorldMap.h"

//-----------------------------------------------------------------
WorldInput::WorldInput()
{
    m_keymap = new Keymap();
    m_keymap->registerKey("quit", KeyStroke(SDLK_ESCAPE, KMOD_NONE));
}
//-----------------------------------------------------------------
WorldInput::~WorldInput()
{
    delete m_keymap;
}
//-----------------------------------------------------------------
/**
 * Process pressed keys and return true to allow continue.
 */
    bool
WorldInput::processInput(WorldMap *world)
{
    bool allowContinue = true;
    if (m_keymap->isPressed("quit")) {
        world->quitState();
        allowContinue = false;
    }

    return allowContinue;
}

