/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "KeyControl.h"


//-----------------------------------------------------------------
/**
 * Default control are arrows.
 */
KeyControl::KeyControl()
{
    m_up = SDLK_UP;
    m_down = SDLK_DOWN;
    m_left = SDLK_LEFT;
    m_right = SDLK_RIGHT;
}

