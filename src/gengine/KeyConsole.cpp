/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "KeyConsole.h"

#include "Log.h"
#include "StringMsg.h"
#include "MessagerAgent.h"
#include "BaseException.h"
#include "ResImageAgent.h"
#include "VideoAgent.h"
#include "Path.h"
#include "LogicException.h"


#include <ctype.h>

//-----------------------------------------------------------------
/**
 * Console starts as deactivated.
 *
 * @throws LogicException when font cannot be created
 */
    KeyConsole::KeyConsole()
: m_history(), m_input(), m_handlerName()
{
    deactivate();

    Path file = Path::dataReadPath("font/console.png");
    SDL_Surface *font_face = ResImageAgent::agent()->loadImage(file);

    m_font = SFont_InitFont(font_face);
    if (NULL == m_font) {
        throw LogicException(ExInfo("cannot create font")
                .addInfo("font", file.getNative()));
    }
}
//-----------------------------------------------------------------
KeyConsole::~KeyConsole()
{
    SFont_FreeFont(m_font);
}
//-----------------------------------------------------------------
    void
KeyConsole::keyDown(const SDL_keysym &keysym)
{
    switch (keysym.sym) {
        case SDLK_UP:
            m_input = m_history;
            break;
        case SDLK_BACKSPACE:
            if (!m_input.empty()) {
                m_input.erase(m_input.end() - 1);
            }
            break;
        case SDLK_RETURN:
            if (!m_input.empty()) {
                if (!m_handlerName.empty()) {
                    try {
                        StringMsg *msg = new StringMsg(m_handlerName,
                                "dostring", m_input);
                        MessagerAgent::agent()->forwardNewMsg(msg);
                        m_history = m_input;
                        //NOTE: clear() is not on FreeBSD
                        m_input = "";
                    }
                    catch (BaseException &e) {
                        //NOTE: debug script fail is not critical
                        LOG_WARNING(e.info());
                    }
                }
            }
            else {
                deactivate();
            }
            break;
        case SDLK_ESCAPE:
            deactivate();
            break;
        default:
            char c = keysym.unicode & 0x7F;
            if (isprint(c)) {
                m_input.push_back(c);
            }
    }
}
//-----------------------------------------------------------------
/**
 * Draw console.
 */
    void
KeyConsole::draw()
{
    SFont_Write(m_screen, m_font, 10, 10,
           ("console] " + m_input).c_str());

}
//-----------------------------------------------------------------
/**
 * Set input handler.
 */
    void
KeyConsole::setHandler(const std::string &handlerName)
{
    m_handlerName = handlerName;
}

