/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "IWidget.h"

#include "MouseStroke.h"

//-----------------------------------------------------------------
/**
 * Call own_mouseButton when stroke is inside own body.
 */
void
IWidget::mouseButton(const MouseStroke &stroke)
{
    if (isInside(stroke.getLoc())) {
        own_mouseButton(stroke);
    }
}
//-----------------------------------------------------------------
/**
 * Returns own_getTip when location is inside own body.
 */
std::string
IWidget::getTip(const V2 &loc)
{
    std::string result;
    if (isInside(loc)) {
        result = own_getTip(loc);
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Test whether location is inside this widget.
 * @param loc world location
 * @return true when location is inside widget body
 */
bool
IWidget::isInside(const V2 &loc)
{
    V2 inside = loc.minus(m_shift);
    return (inside.getX() >= 0 && inside.getX() < getW()
        && inside.getY() >= 0 && inside.getY() < getH());
}

