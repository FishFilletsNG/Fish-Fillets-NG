/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SelectLang.h"

#include "LangFlag.h"
#include "RectBinder.h"

#include "def-script.h"
#include "minmax.h"
#include "Path.h"
#include "ScriptState.h"
#include "MessagerAgent.h"
#include "OptionAgent.h"
#include "StringMsg.h"
#include "UnknownMsgException.h"
#include "MouseStroke.h"

//-----------------------------------------------------------------
    inline SelectLang *
getSelect(lua_State *L)
{
    return static_cast<SelectLang*>(script_getLeader(L));
}
//-----------------------------------------------------------------
/**
 * void select_addFlag(lang, picture)
 */
    int
script_select_addFlag(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *lang = luaL_checkstring(L, 1);
    const char *picture = luaL_checkstring(L, 2);

    getSelect(L)->addFlag(new LangFlag(lang, Path::dataReadPath(picture)));
    END_NOEXCEPTION;
    return 0;
}

//-----------------------------------------------------------------
/**
 * Execute script which will add flags.
 */
    SelectLang::SelectLang(const Path &datafile)
: m_shift(0, 0)
{
    m_flagW = 0;
    m_flagH = 0;

    ScriptState script;
    script.registerLeader(this);
    script.registerFunc("select_addFlag", script_select_addFlag);
    script.doFile(datafile);

    m_rectBinder = new RectBinder();
}
//-----------------------------------------------------------------
/**
 * Release all flags.
 */
SelectLang::~SelectLang()
{
    delete m_rectBinder;
    t_flags::iterator end = m_flags.end();
    for (t_flags::iterator i = m_flags.begin(); i != end; ++i) {
        delete *i;
    }
    MessagerAgent::agent()->removeListener(getName());
}
//-----------------------------------------------------------------
    void
SelectLang::addFlag(LangFlag *new_flag)
{
    m_flags.push_back(new_flag);
    m_flagW = max(m_flagW, new_flag->getW());
    m_flagH = max(m_flagH, new_flag->getH());
}
//-----------------------------------------------------------------
/**
 * Register active rects for all added flags.
 */
    void
SelectLang::initFlags()
{
    registerRects();
    MessagerAgent::agent()->addListener(this);
}
//-----------------------------------------------------------------
    int
SelectLang::flagsPerRow() const
{
    return max(1, MAX_WIDTH / selectedW());
}
//-----------------------------------------------------------------
    int
SelectLang::flagsPerCol() const
{
    bool remain = m_flags.size() % flagsPerRow();
    return m_flags.size() / flagsPerRow() + remain;
}
//-----------------------------------------------------------------
int
SelectLang::getW() const
{
    return selectedW() * min(flagsPerRow(), m_flags.size());
}
//-----------------------------------------------------------------
int
SelectLang::getH() const
{
    return selectedH() * flagsPerCol();
}
//-----------------------------------------------------------------
/**
 * Add all flags as rects to rectBinder.
 * NOTE: rect has FLAG_MARGIN overlap.
 */
    void
SelectLang::registerRects()
{
    int perRow = flagsPerRow();
    int col = 0;
    int row = 0;

    t_flags::iterator end = m_flags.end();
    for (t_flags::iterator i = m_flags.begin(); i != end; ++i) {
        SDL_Rect rect;
        rect.x = col * selectedW();
        rect.y = row * selectedH();
        rect.w = selectedW();
        rect.h = selectedH();
        m_rectBinder->addRect(rect, createSetMsg((*i)->getLang()));

        col++;
        if (col > perRow) {
            col = 0;
            row++;
        }
    }
}
//-----------------------------------------------------------------
/**
 * Create message to set lang.
 */
    BaseMsg *
SelectLang::createSetMsg(const std::string &lang)
{
    return new StringMsg(this, "lang", lang);
}
//-----------------------------------------------------------------
/**
 * Draw all flag relative to own shift.
 */
    void
SelectLang::draw()
{
    int perRow = flagsPerRow();
    int col = 0;
    int row = 0;

    V2 margin(FLAG_MARGIN, FLAG_MARGIN);
    std::string selected = OptionAgent::agent()->getParam("lang");
    t_flags::iterator end = m_flags.end();
    for (t_flags::iterator i = m_flags.begin(); i != end; ++i) {
        V2 loc(col * selectedW(),
                row * selectedH());
        loc = loc.plus(m_shift);
        if (selected == (*i)->getLang()) {
            drawSelected(loc);
        }
        (*i)->drawOn(m_screen, loc.plus(margin));

        col++;
        if (col > perRow) {
            col = 0;
            row++;
        }
    }
}
//-----------------------------------------------------------------
/**
 * Draw border for selected button.
 * @param loc button location
 */
    void
SelectLang::drawSelected(const V2 &loc)
{
    SDL_Rect rect;
    rect.x = loc.getX();
    rect.y = loc.getY();
    rect.w = selectedW();
    rect.h = selectedH();

    Uint32 yellow = SDL_MapRGB(m_screen->format, 0xff, 0xff, 0x00);
    SDL_FillRect(m_screen, &rect, yellow);
}
//-----------------------------------------------------------------
/**
 * React to mouse button press.
 * NOTE: flags coordinates are relative,
 * rectBinder will need to known shift relative to world
 */
    void
SelectLang::mouseButton(const MouseStroke &stroke)
{
    V2 loc = stroke.getLoc();
    loc = loc.minus(m_shift);
    m_rectBinder->mouseDown(loc);
}
//-----------------------------------------------------------------
/**
 * Messages:
 * - lang("lang") ... set this lang as persistent option
 *
 * @throws UnknownMsgException
 */
    void
SelectLang::receiveString(const StringMsg *msg)
{
    if (msg->equalsName("lang")) {
        OptionAgent::agent()->setPersistent("lang", msg->getValue());
    }
    else {
        throw UnknownMsgException(msg);
    }
}

