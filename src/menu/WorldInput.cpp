/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WorldInput.h"

#include "WorldMap.h"
#include "Keymap.h"

#include "Log.h"
#include "KeyStroke.h"
#include "MouseStroke.h"

//-----------------------------------------------------------------
WorldInput::WorldInput(WorldMap *world)
{
    m_world = world;
    m_keymap = new Keymap();
    m_keymap->registerKey(KeyStroke(SDLK_ESCAPE, KMOD_NONE),
            KeyDesc(KEY_QUIT, "quit"));
}
//-----------------------------------------------------------------
WorldInput::~WorldInput()
{
    delete m_keymap;
}
//-----------------------------------------------------------------
    void
WorldInput::keyEvent(const KeyStroke &stroke)
{
    //TODO: 'O' ... options, 'I' ... intro, 'E' ... exit, 'C' ... credits
    switch (m_keymap->indexPressed(stroke)) {
        case KEY_QUIT:
            m_world->quitState();
            break;
        case -1:
            break;
        default:
            LOG_WARNING(ExInfo("unknown key")
                    .addInfo("index", m_keymap->indexPressed(stroke))
                    .addInfo("stroke", stroke.toString()));
    }
}
//-----------------------------------------------------------------
void
WorldInput::mouseEvent(const MouseStroke &buttons)
{
    if (buttons.isLeft()) {
        m_world->runSelected();
    }
}

