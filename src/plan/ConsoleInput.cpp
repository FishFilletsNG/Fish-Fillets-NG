/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ConsoleInput.h"

#include "Keymap.h"
#include "KeyConsole.h"

#include "Log.h"
#include "KeyDesc.h"
#include "KeyStroke.h"

#include <ctype.h> // isprint()

//-----------------------------------------------------------------
ConsoleInput::ConsoleInput(KeyConsole *console)
    : StateInput(console)
{
    KeyDesc key_history(KEY_HISTORY, "input history");
    KeyDesc key_backspace(KEY_BACKSPACE, "backspace");
    KeyDesc key_clear(KEY_CLEAR, "clear");
    KeyDesc key_enter(KEY_ENTER, "enter");

    m_keymap->registerKey(KeyStroke(SDLK_UP, KMOD_NONE), key_history);
    m_keymap->registerKey(KeyStroke(SDLK_BACKSPACE, KMOD_NONE), key_backspace);
    m_keymap->registerKey(KeyStroke(SDLK_u, KMOD_LCTRL), key_clear);
    m_keymap->registerKey(KeyStroke(SDLK_u, KMOD_RCTRL), key_clear);
    m_keymap->registerKey(KeyStroke(SDLK_RETURN, KMOD_NONE), key_enter);
}
//-----------------------------------------------------------------
KeyConsole *
ConsoleInput::getConsole()
{
    return dynamic_cast<KeyConsole*>(m_state);
}
//-----------------------------------------------------------------
/**
 * Toggle console.
 */
void
ConsoleInput::enableConsole()
{
    quitState();
}
//-----------------------------------------------------------------
    void
ConsoleInput::specKey(int keyIndex)
{
    //TODO: simulate key repeat in console
    switch (keyIndex) {
        case KEY_HISTORY:
            getConsole()->setInput(m_history);
            break;
        case KEY_BACKSPACE:
            {
                std::string input = getConsole()->getInput();
                if (!input.empty()) {
                    input.erase(input.end() - 1);
                    getConsole()->setInput(input);
                }
            }
            break;
        case KEY_CLEAR:
            getConsole()->setInput("");
            break;
        case KEY_ENTER:
            {
                std::string input = getConsole()->getInput();
                if (!input.empty()) {
                    if (getConsole()->sendCommand()) {
                        m_history = input;
                        getConsole()->setInput("");
                    }
                }
                else {
                    quitState();
                }
            }
            break;
        default:
            StateInput::specKey(keyIndex);
            break;
    }
}
//-----------------------------------------------------------------
void
ConsoleInput::specStroke(const KeyStroke &stroke)
{
    //TODO: support UTF-8
    char c = stroke.getUnicode() & 0x7F;
    if (isprint(c)) {
        std::string input = getConsole()->getInput();
        input.append(1, c);
        getConsole()->setInput(input);
    }
}

