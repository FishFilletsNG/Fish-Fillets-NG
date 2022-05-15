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
    m_up = SDL_SCANCODE_UP;
    m_down = SDL_SCANCODE_DOWN;
    m_left = SDL_SCANCODE_LEFT;
    m_right = SDL_SCANCODE_RIGHT;
}

