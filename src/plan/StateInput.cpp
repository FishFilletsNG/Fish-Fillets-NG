/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "StateInput.h"

#include "Keymap.h"
#include "GameState.h"

#include "Log.h"
#include "KeyStroke.h"
#include "KeyConsole.h"
#include "OptionAgent.h"

//-----------------------------------------------------------------
/**
 * Create state input handler.
 * @param state pointer to the leader
 */
StateInput::StateInput(GameState *state)
{
    m_state = state;
    m_keymap = new Keymap();
    KeyDesc key_quit(KEY_QUIT, "quit");
    KeyDesc key_console(KEY_CONSOLE, "debug console");
    KeyDesc key_help(KEY_HELP, "help screen");
    KeyDesc key_menu(KEY_MENU, "game menu");
    m_keymap->registerKey(KeyStroke(SDLK_ESCAPE, KMOD_NONE), key_quit);
    m_keymap->registerKey(KeyStroke(SDLK_BACKQUOTE, KMOD_NONE), key_console);
    m_keymap->registerKey(KeyStroke(SDLK_F1, KMOD_NONE), key_help);
    m_keymap->registerKey(KeyStroke(SDLK_F10, KMOD_NONE), key_menu);
    m_keymap->registerKey(KeyStroke(SDLK_F6, KMOD_NONE),
            KeyDesc(KEY_SUBTITLES, "enable subtitles"));
}
//-----------------------------------------------------------------
StateInput::~StateInput()
{
    delete m_keymap;
}
//-----------------------------------------------------------------
/**
 * Inspects keystroke and call:
 * - common function for quit, console, help, menu, subtitles
 * - specKey() for defined keystroke but unknown for us
 * - specStroke() for other keys
 */
    void
StateInput::keyEvent(const KeyStroke &stroke)
{
    int index = m_keymap->indexPressed(stroke);
    switch (index) {
        case KEY_QUIT:
            quitState();
            break;
        case KEY_CONSOLE:
            enableConsole();
            break;
        case KEY_HELP:
            enableHelp();
            break;
        case KEY_MENU:
            enableMenu();
            break;
        case KEY_SUBTITLES:
            enableSubtitles();
            break;
        case -1:
            specStroke(stroke);
            break;
        default:
            specKey(index);
            break;
    }
}
//-----------------------------------------------------------------
    void
StateInput::specKey(int keyIndex)
{
    LOG_WARNING(ExInfo("unknown key")
            .addInfo("keyIndex", keyIndex));
}
//-----------------------------------------------------------------
void
StateInput::quitState()
{
    m_state->quitState();
}
//-----------------------------------------------------------------
    void
StateInput::enableConsole()
{
    m_state->pushState(new KeyConsole());
}
//-----------------------------------------------------------------
    void
StateInput::enableSubtitles()
{
    toggleParam("subtitles");
}
//-----------------------------------------------------------------
    void
StateInput::toggleParam(const std::string &param)
{
    OptionAgent *option = OptionAgent::agent();
    bool enable = option->getAsBool(param);

    option->setPersistent(param, !enable);
}

