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

#include "KeyStroke.h"
#include "DemoMode.h"

//-----------------------------------------------------------------
DemoInput::DemoInput()
{
    m_keymap = new Keymap();
    m_keymap->registerKey("quit", KeyStroke(SDLK_ESCAPE, KMOD_NONE));
    m_keymap->registerKey("interrupt", KeyStroke(SDLK_SPACE, KMOD_NONE));
}
//-----------------------------------------------------------------
DemoInput::~DemoInput()
{
    delete m_keymap;
}
//-----------------------------------------------------------------
/**
 * Process pressed keys and return true to allow continue.
 */
bool
DemoInput::processInput(DemoMode *demo)
{
    bool allowContinue = true;
    if (m_keymap->isPressed("quit") ||
            m_keymap->isPressed("interrupt"))
    {
        demo->quitState();
        allowContinue = false;
    }

    return allowContinue;
}

