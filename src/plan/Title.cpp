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
#include "OptionAgent.h"
#include "StringTool.h"

//-----------------------------------------------------------------
/**
 * Create new title to draw.
 * X is centred.
 * Y is baseY above bottom screen border.
 *
 * @param baseY number of pixels from the bottom border
 * @param finalY final position, it changes when next subtitle is added
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

    m_screenW = OptionAgent::agent()->getAsInt("screen_width");
    m_screenH = OptionAgent::agent()->getAsInt("screen_height");
    m_x = (m_screenW - text_width) / 2;
    m_y = m_screenH - baseY;
    m_finalY = m_screenH - finalY;
    m_limitY = m_screenH - limitY;
    m_mintime = StringTool::utf8Length(m_content) * TIME_PER_CHAR;
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
 */
    void
Title::drawOn(SDL_Surface *screen)
{
    //TODO: wavy text
    SDL_Rect rect;
    rect.x = m_x;
    rect.y = m_y;

    SDL_BlitSurface(m_surface, NULL, screen, &rect);
}
//-----------------------------------------------------------------
/**
 * Shift up until title is on limit.
 * Decrease m_mintime.
 */
    void
Title::shiftUp(int rate)
{
    m_mintime--;
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
    return m_screenH - m_y;
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

