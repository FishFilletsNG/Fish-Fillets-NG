/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "OptionsInput.h"

#include "MenuOptions.h"
#include "Keymap.h"

#include "Log.h"
#include "KeyStroke.h"
#include "MouseStroke.h"

//-----------------------------------------------------------------
/**
 * Only function key is Escape->quit.
 */
OptionsInput::OptionsInput(MenuOptions *menu)
{
    m_menu = menu;
    m_keymap = new Keymap();
    m_keymap->registerKey(KeyStroke(SDLK_ESCAPE, KMOD_NONE),
            KeyDesc(KEY_QUIT, "quit"));
    m_keymap->registerKey(KeyStroke(SDLK_F10, KMOD_NONE),
            KeyDesc(KEY_QUIT, "quit"));
}
//-----------------------------------------------------------------
OptionsInput::~OptionsInput()
{
    delete m_keymap;
}
//-----------------------------------------------------------------
    void
OptionsInput::keyEvent(const KeyStroke &stroke)
{
    switch (m_keymap->indexPressed(stroke)) {
        case KEY_QUIT:
            m_menu->quitState();
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
OptionsInput::mouseEvent(const MouseStroke &buttons)
{
    m_menu->mouseButton(buttons);
}

