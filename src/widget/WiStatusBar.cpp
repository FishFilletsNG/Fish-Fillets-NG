/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WiStatusBar.h"

#include "Font.h"

//-----------------------------------------------------------------
WiStatusBar::WiStatusBar(Font *new_font, const SDL_Color &color, int width)
: m_color(color)
{
    m_font = new_font;
    m_w = width;
}
//-----------------------------------------------------------------
WiStatusBar::~WiStatusBar()
{
    delete m_font;
}
//-----------------------------------------------------------------
int
WiStatusBar::getH() const
{
    return m_font->getHeight();
}
//-----------------------------------------------------------------
void
WiStatusBar::drawOn(SDL_Surface *screen)
{
    if (!m_label.empty()) {
        SDL_Rect rect;
        rect.x = m_shift.getX();
        rect.y = m_shift.getY();

        SDL_Surface *rendered = m_font->renderTextOutlined(m_label, m_color);
        SDL_BlitSurface(rendered, NULL, screen, &rect);
        SDL_FreeSurface(rendered);
    }
}

