/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LevelInput.h"

#include "Keymap.h"

#include "KeyStroke.h"
#include "Level.h"

//-----------------------------------------------------------------
LevelInput::LevelInput()
{
    //TODO: help on F1
    m_keymap = new Keymap();
    m_keymap->registerKey("quit", KeyStroke(SDLK_ESCAPE, KMOD_NONE));
    m_keymap->registerKey("switch", KeyStroke(SDLK_SPACE, KMOD_NONE));
    m_keymap->registerKey("save", KeyStroke(SDLK_F2, KMOD_NONE));
    m_keymap->registerKey("load", KeyStroke(SDLK_F3, KMOD_NONE));
    m_keymap->registerKey("restart", KeyStroke(SDLK_BACKSPACE, KMOD_NONE));
}
//-----------------------------------------------------------------
LevelInput::~LevelInput()
{
    delete m_keymap;
}
//-----------------------------------------------------------------
/**
 * Process pressed keys and return true to allow continue.
 */
bool
LevelInput::processInput(Level *level)
{
    bool allowContinue = true;
    if (m_keymap->isPressed("restart")) {
        level->interruptPlan();
        level->action_restart();
    }
    else if (m_keymap->isPressed("load")) {
        level->interruptPlan();
        level->action_load();
    }
    else if (m_keymap->isPressed("save")) {
        if (!level->isPlanning()) {
            level->action_save();
        }
    }
    else if (m_keymap->isPressed("switch")) {
        if (!level->isPlanning()) {
            level->switchFish();
        }
    }
    else if (m_keymap->isPressed("quit")) {
        level->quitState();
        allowContinue = false;
    }

    return allowContinue;
}

