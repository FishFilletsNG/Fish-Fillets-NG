/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "HelpInput.h"

#include "Keymap.h"
#include "KeyDesc.h"
#include "GameState.h"

//-----------------------------------------------------------------
/**
 * Escape, space or mouse click quit state.
 */
HelpInput::HelpInput(GameState *state)
    : StateInput(state)
{
}
//-----------------------------------------------------------------
/**
 * Quits on unhandled keypress.
 */
void
HelpInput::specStroke(const KeyStroke &/*stroke*/)
{
    quitState();
}
//-----------------------------------------------------------------
void
HelpInput::mouseEvent(const MouseStroke &/*buttons*/)
{
    quitState();
}

