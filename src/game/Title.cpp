/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Title.h"

#include "Log.h"
#include "OptionAgent.h"

//-----------------------------------------------------------------
/**
 * Create new title to draw.
 * X is centred.
 * Y is baseY above bottom screen border.
 */
Title::Title(int baseY, int finalY, int bonusTime, int minY,
        const std::string &content, const SFont_Font *font)
: m_content(content)
{
    m_font = font;

    int screen_width = OptionAgent::agent()->getAsInt("screen_width");
    int screen_height = OptionAgent::agent()->getAsInt("screen_height");
    int text_width = SFont_TextWidth(m_font, content.c_str());

    m_x = (screen_width - text_width) / 2;
    m_y = screen_height - baseY;
    m_finalY = screen_height - finalY;
    m_minY = screen_height - minY;
    m_mintime = m_content.size() * TIME_PER_CHAR;
    if (m_mintime < TIME_MIN) {
        m_mintime = TIME_MIN;
    }
    m_mintime += bonusTime;
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
    SFont_Write(m_screen, m_font, m_x, m_y, m_content.c_str());
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
    int screen_height = OptionAgent::agent()->getAsInt("screen_height");
    return screen_height - m_y;
}
//-----------------------------------------------------------------
/**
 * Whether title was long enough on screen.
 */
    bool
Title::isGone()
{
    return (m_mintime < 0 || m_y < m_minY);
}

