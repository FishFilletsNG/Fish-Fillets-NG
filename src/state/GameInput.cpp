/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "GameInput.h"

#include "KeyConsole.h"
#include "MenuOptions.h"

//-----------------------------------------------------------------
/**
 * Push menu state at top.
 */
void
GameInput::enableMenu()
{
    m_state->pushState(new MenuOptions());
}

