/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SubTitleAgent.h"

#include "Title.h"
#include "Font.h"
#include "ResColorPack.h"

#include "Path.h"
#include "OptionAgent.h"
#include "minmax.h"

//-----------------------------------------------------------------
    void
SubTitleAgent::own_init()
{
    m_limitY = TITLE_LIMIT_Y;
    m_colors = new ResColorPack();

    m_font = NULL;
    m_font = new Font(Path::dataReadPath("font/font_subtitle.ttf"), 20);
}
//-----------------------------------------------------------------
/**
 * Shift all titles up.
 * Try remove the oldest subtitle.
 */
    void
SubTitleAgent::own_update()
{
    if (!m_titles.empty()) {
        shiftTitlesUp(TITLE_SPEED);

        if (m_titles.front()->isGone()) {
            delete m_titles.front();
            m_titles.pop_front();
        }
    }
}
//-----------------------------------------------------------------
    void
SubTitleAgent::own_shutdown()
{
    removeAll();
    delete m_colors;
    if (m_font) {
        delete m_font;
    }
}

//-----------------------------------------------------------------
    void
SubTitleAgent::addFont(const std::string &fontname, Color *new_color)
{
    m_colors->addRes(fontname, new_color);
}
//-----------------------------------------------------------------
/**
 * Create new subtitle.
 * Shift all existing subtitles up.
 */
void
SubTitleAgent::newSubtitle(const std::string &original,
        const std::string &fontname)
{
    const Color *color = m_colors->getRes(fontname);

    std::string subtitle = original;
    while (!subtitle.empty()) {
        subtitle = splitAndCreate(subtitle, color);
    }
}
//-----------------------------------------------------------------
/**
 * Split subtitle, create title and return rest.
 * @return rest or empty string
 */
std::string
SubTitleAgent::splitAndCreate(const std::string &original,
        const Color *color)
{
    std::string subtitle = original;
    int screen_width = OptionAgent::agent()->getAsInt("screen_width");

    int text_width = m_font->calcTextWidth(subtitle);
    while (text_width > screen_width - 2 * TITLE_BORDER) {
        trimRest(subtitle);
        text_width = m_font->calcTextWidth(subtitle);
    }

    std::string rest = "";
    if (!subtitle.empty()) {
        newShortSubtitle(subtitle, color);

        if (original.size() > subtitle.size()) {
            rest = original.substr(subtitle.size());
        }
    }
    return rest;
}
//-----------------------------------------------------------------
/**
 * Break long string.
 * String is trimed at ' '
 * but not at " . " (single char surrounded with spaces).
 *
 * @param buffer buffer to change
 */
void
SubTitleAgent::trimRest(std::string &buffer)
{
    int i;
    for (i = buffer.size() - 1; i >= 0; --i) {
        if (buffer[i] == ' ' &&
                !(i - 2 >= 0 && buffer[i - 2] == ' '))
        {
            break;
        }
    }

    if (i <= 0) {
        LOG_WARNING(ExInfo("unbreakable string")
                .addInfo("string", buffer));
        if (buffer.size() > 4) {
            buffer.erase(buffer.size() - 4);
        }
        else {
            buffer = "";
        }
    }
    else {
        buffer.erase(i);
    }
}
//-----------------------------------------------------------------
    void
SubTitleAgent::newShortSubtitle(const std::string &subtitle,
        const Color *color)
{
    int startY = lowestY();
    int finalY = TITLE_BASE + TITLE_ROW;
    int bonusTime = (TITLE_BASE - startY + m_limitY - TITLE_LIMIT_Y)
        / TITLE_SPEED;
    Title *title = new Title(startY, finalY, bonusTime, m_limitY,
            subtitle, m_font, color);
    shiftFinalsUp(TITLE_ROW);
    m_titles.push_back(title);
}
//-----------------------------------------------------------------
/**
 * Increase Y for all existing titles.
 */
    void
SubTitleAgent::shiftTitlesUp(int rate)
{
    t_titles::iterator end = m_titles.end();
    for (t_titles::iterator i = m_titles.begin(); i != end; ++i) {
        (*i)->shiftUp(rate);
    }
}
//-----------------------------------------------------------------
/**
 * Increase finalY for all existing titles.
 */
    void
SubTitleAgent::shiftFinalsUp(int rate)
{
    t_titles::iterator end = m_titles.end();
    for (t_titles::iterator i = m_titles.begin(); i != end; ++i) {
        (*i)->shiftFinalUp(rate);
    }
}
//-----------------------------------------------------------------
/**
 * Get lowest possible Y.
 * It can be negative.
 */
    int
SubTitleAgent::lowestY()
{
    int lowest = TITLE_BASE;
    if (!m_titles.empty()) {
        int lastest = m_titles.back()->getY() - TITLE_ROW;
        lowest = min(lowest, lastest);
    }
    return lowest;
}
//-----------------------------------------------------------------
/**
 * Kill all running subtitles.
 */
void
SubTitleAgent::killTalks()
{
    t_titles::iterator end = m_titles.end();
    for (t_titles::iterator i = m_titles.begin(); i != end; ++i) {
        delete *i;
    }
    m_titles.clear();
}
//-----------------------------------------------------------------
/**
 * Kill all subtitles and remove fonts.
 */
    void
SubTitleAgent::removeAll()
{
    killTalks();
    m_colors->removeAll();
}

//-----------------------------------------------------------------
/**
 * Draw all subtitles.
 */
void
SubTitleAgent::drawOn(SDL_Surface *screen)
{
    if (OptionAgent::agent()->getAsBool("subtitles", true)) {
        t_titles::iterator end = m_titles.end();
        for (t_titles::iterator i = m_titles.begin(); i != end; ++i) {
            (*i)->drawOn(screen);
        }
    }
}
