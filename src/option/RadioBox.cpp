/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "RadioBox.h"

#include "WiPicture.h"
#include "OptionAgent.h"
#include "MouseStroke.h"

//-----------------------------------------------------------------
RadioBox::RadioBox(const std::string &param, const std::string &value,
        const Path &picture)
: WiContainer(new WiPicture(picture), BORDER), m_param(param), m_value(value)
{
}
//-----------------------------------------------------------------
/**
 * Draw border for selected button.
 */
    void
RadioBox::drawOn(SDL_Surface *screen)
{
    if (OptionAgent::agent()->getParam(m_param) == m_value) {
        SDL_Rect rect;
        rect.x = m_shift.getX();
        rect.y = m_shift.getY();
        rect.w = getW();
        rect.h = getH();

        Uint32 green = SDL_MapRGB(screen->format, 0x00, 0xff, 0x00);
        SDL_FillRect(screen, &rect, green);
    }
    WiContainer::drawOn(screen);
}
//-----------------------------------------------------------------
/**
 * Select button and set param=value.
 */
    void
RadioBox::own_mouseButton(const MouseStroke &stroke)
{
    if (stroke.isLeft()) {
        OptionAgent::agent()->setPersistent(m_param, m_value);
    }
}

