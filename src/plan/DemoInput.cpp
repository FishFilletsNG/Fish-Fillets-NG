/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "DemoInput.h"

#include "Keymap.h"

#include "Log.h"
#include "KeyStroke.h"
#include "DemoMode.h"

//-----------------------------------------------------------------
/**
 * Create demo input handler.
 * @param demo pointer to the leader
 */
DemoInput::DemoInput(DemoMode *demo)
{
    m_demo = demo;
    m_keymap = new Keymap();
    KeyDesc key_quit(KEY_QUIT, "quit");
    m_keymap->registerKey(KeyStroke(SDLK_ESCAPE, KMOD_NONE), key_quit);
    m_keymap->registerKey(KeyStroke(SDLK_SPACE, KMOD_NONE), key_quit);
}
//-----------------------------------------------------------------
DemoInput::~DemoInput()
{
    delete m_keymap;
}
//-----------------------------------------------------------------
    void
DemoInput::keyEvent(const KeyStroke &stroke)
{
    switch (m_keymap->indexPressed(stroke)) {
        case KEY_QUIT:
            m_demo->quitState();
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
DemoInput::mouseEvent(const MouseStroke &/*buttons*/)
{
    m_demo->quitState();
}

