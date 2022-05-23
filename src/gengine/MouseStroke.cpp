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
    : m_loc(event.x, event.y), m_doScale(true)
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

float MouseStroke::scaleX = 1.0;
float MouseStroke::scaleY = 1.0;
V2 MouseStroke::letterbox = V2(0, 0);
V2 MouseStroke::logicalRes = V2(0, 0);
V2 MouseStroke::renderRes = V2(0, 0);
void MouseStroke::setScale(float x, float y)
{
    assert(x > 0 && y > 0);
    scaleX=x;
    scaleY=y;
}
void MouseStroke::getScale(float& x, float& y)
{
    x=scaleX;
    y=scaleY;
}
// Only needed for options menu in fullscreen
void MouseStroke::disableScaling()
{
    m_doScale = false;
}
void MouseStroke::setLetterbox(V2 letterbox)
{
    MouseStroke::letterbox = letterbox;
}
void MouseStroke::setResolution(V2 logical, V2 render)
{
    MouseStroke::logicalRes = logical;
    MouseStroke::renderRes = render;
}
V2 MouseStroke::getLoc() const
{
    if (m_doScale) {
        return V2((m_loc.getX() - letterbox.getX()) * scaleX, (m_loc.getY() - letterbox.getY()) * scaleY);
    }
    else {
        // in options menu, probably
        return m_loc;
    }
}
