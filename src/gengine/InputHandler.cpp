/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "InputHandler.h"

#include "ExInfo.h"

//-----------------------------------------------------------------
InputHandler::InputHandler()
: m_mouseLoc(-1, -1)
{
    m_pressed = NULL;
    m_buttons = 0;
}
//-----------------------------------------------------------------
void
InputHandler::mouseState(const V2 &loc, Uint8 buttons)
{
    m_mouseLoc = loc;
    m_buttons = buttons;
}
//-----------------------------------------------------------------
bool
InputHandler::isPressed(SDLKey key) const
{
    return m_pressed && m_pressed[key];
}
//-----------------------------------------------------------------
bool
InputHandler::isLeftPressed() const
{
    return m_buttons & SDL_BUTTON(1);
}
//-----------------------------------------------------------------
bool
InputHandler::isMiddlePressed() const
{
    return m_buttons & SDL_BUTTON(2);
}
//-----------------------------------------------------------------
bool
InputHandler::isRightPressed() const
{
    return m_buttons & SDL_BUTTON(3);
}
//-----------------------------------------------------------------
std::string
InputHandler::toString() const
{
    return ExInfo("input")
        .addInfo("mouse", m_mouseLoc.toString())
        .addInfo("buttons", m_buttons).info();
}

