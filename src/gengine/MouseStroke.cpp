/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "MouseStroke.h"

#include "StringTool.h"

//-----------------------------------------------------------------
/**
 * Create new stroke from MouseButtonEvent.
 */
MouseStroke::MouseStroke(const SDL_MouseButtonEvent &event)
    : m_loc(event.x, event.y)
{
    m_button = event.button;
}
//-----------------------------------------------------------------
/**
 * Return text fashion.
 */
std::string
MouseStroke::toString() const
{
    return StringTool::toString(m_button);
}

