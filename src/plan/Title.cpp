/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Title.h"

#include "Font.h"
#include "Color.h"

//-----------------------------------------------------------------
/**
 * Create new title to draw.
 * X is centred.
 * Y is baseY above bottom screen border.
 *
 * @param baseY number of pixels from the bottom border
 * @param finalY final position, it change when next subtitle is added
 * @param bonusTime bonus time for subtitle under bottom border
 * @param limitY max Y distance from bottom border
 * @param content subtitle content
 * @param font subtitle font (shared)
 * @param color subtitle color (shared)
 */
Title::Title(int baseY, int finalY, int bonusTime, int limitY,
        const std::string &content, Font *font, const Color *color)
: m_content(content)
{
    m_font = font;
    m_surface = m_font->renderTextOutlined(content, *color);

    int text_width = m_font->calcTextWidth(content);

    m_x = (m_screen->w - text_width) / 2;
    m_y = m_screen->h - baseY;
    m_finalY = m_screen->h - finalY;
    m_limitY = m_screen->h - limitY;
    m_mintime = m_content.size() * TIME_PER_CHAR;
    if (m_mintime < TIME_MIN) {
        m_mintime = TIME_MIN;
    }
    m_mintime += bonusTime;
}
//-----------------------------------------------------------------
Title::~Title()
{
    SDL_FreeSurface(m_surface);
}
//-----------------------------------------------------------------
/**
 * Draw model.
 * Decrease m_mintime.
 */
    void
Title::draw()
{
    //TODO: wavy text
    SDL_Rect rect;
    rect.x = m_x;
    rect.y = m_y;

    SDL_BlitSurface(m_surface, NULL, m_screen, &rect);
    m_mintime--;
}
//-----------------------------------------------------------------
    void
Title::shiftUp(int rate)
{
    m_y -= rate;
    if (m_y < m_finalY) {
        m_y = m_finalY;
    }
}
//-----------------------------------------------------------------
    void
Title::shiftFinalUp(int rate)
{
    m_finalY -= rate;
}
//-----------------------------------------------------------------
/**
 * Return Y position from the bottom border.
 */
int
Title::getY() const
{
    return m_screen->h - m_y;
}
//-----------------------------------------------------------------
/**
 * Whether title was long enough on screen.
 */
    bool
Title::isGone()
{
    return (m_mintime < 0 || m_y < m_limitY);
}

