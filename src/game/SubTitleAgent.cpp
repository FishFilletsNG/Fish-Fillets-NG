/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SubTitleAgent.h"

#include "ResFontPack.h"
#include "Title.h"
#include "OptionAgent.h"

//-----------------------------------------------------------------
    void
SubTitleAgent::own_init()
{
    m_fonts = new ResFontPack();
}
//-----------------------------------------------------------------
/**
 * Shift all titles up.
 * Try remove the oldest subtitle.
 */
    void
SubTitleAgent::own_update()
{
    if (false == m_titles.empty()) {
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
    delete m_fonts;
}

//-----------------------------------------------------------------
    void
SubTitleAgent::addFont(const std::string &fontname, const Path &file)
{
    m_fonts->addFont(fontname, file);
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
    //TODO: split long subtitle to more rows
    SFont_Font *font = m_fonts->getRes(fontname);

    std::string subtitle = original;
    while (false == subtitle.empty()) {
        subtitle = splitAndCreate(subtitle, font);
    }
}
//-----------------------------------------------------------------
/**
 * Split subtitle, create title and return rest.
 * @return rest or empty string
 */
std::string
SubTitleAgent::splitAndCreate(const std::string &original,
        SFont_Font *font)
{
    std::string subtitle = original;
    int screen_width = OptionAgent::agent()->getAsInt("screen_width");

    int text_width = SFont_TextWidth(font, subtitle.c_str());
    while (text_width > screen_width - 2 * TITLE_BORDER) {
        trimRest(subtitle);
        text_width = SFont_TextWidth(font, subtitle.c_str());
    }

    std::string rest = "";
    if (false == subtitle.empty()) {
        newShortSubtitle(subtitle, font);

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
                false == (i - 2 >= 0 && buffer[i - 2] == ' '))
        {
            break;
        }
    }

    if (i <= 0) {
        LOG_WARNING(ExInfo("unbreakable string")
                .addInfo("string", buffer));
        buffer = "";
    }
    else {
        buffer.erase(i);
    }
}
//-----------------------------------------------------------------
    void
SubTitleAgent::newShortSubtitle(const std::string &subtitle,
        SFont_Font *font)
{
    int startY = lowestY();
    int finalY = TITLE_BASE + TITLE_ROW;
    int bonusTime = (TITLE_BASE - startY) / TITLE_SPEED;
    Title *title = new Title(startY, finalY, bonusTime, TITLE_LIMIT_Y,
            subtitle, font);
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
    if (false == m_titles.empty()) {
        int lastest = m_titles.back()->getY() - TITLE_ROW;
        //TODO: offer MIN macro
        lowest = (lowest <= lastest ? lowest : lastest);
    }
    return lowest;
}
//-----------------------------------------------------------------
    void
SubTitleAgent::removeAll()
{
    t_titles::iterator end = m_titles.end();
    for (t_titles::iterator i = m_titles.begin(); i != end; ++i) {
        delete *i;
    }
    m_titles.clear();

    m_fonts->removeAll();
}
