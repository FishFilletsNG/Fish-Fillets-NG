/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "KeyConsole.h"

#include "Font.h"

#include "Log.h"
#include "StringMsg.h"
#include "MessagerAgent.h"
#include "BaseException.h"
#include "VideoAgent.h"
#include "Path.h"


#include <ctype.h>

//-----------------------------------------------------------------
/**
 * Console starts as deactivated.
 */
    KeyConsole::KeyConsole()
: m_history(), m_input(), m_handlerName(), m_color(0, 200, 0)
{
    deactivate();

    m_font = new Font(Path::dataReadPath("font/font_console.ttf"), 16);
}
//-----------------------------------------------------------------
KeyConsole::~KeyConsole()
{
    delete m_font;
}
//-----------------------------------------------------------------
    void
KeyConsole::keyDown(const SDL_keysym &keysym)
{
    //TODO: simulate key repeat in console
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
        case SDLK_BACKQUOTE:
            deactivate();
            break;
        default:
            char c = keysym.unicode & 0x7F;
            if (isprint(c)) {
                m_input.push_back(c);
            }
            break;
    }
}
//-----------------------------------------------------------------
/**
 * Draw console.
 */
    void
KeyConsole::draw()
{
    SDL_Rect rect;
    rect.x = 10;
    rect.y = 10;

    SDL_Surface *surface = m_font->renderText("console] " + m_input, m_color);
    SDL_BlitSurface(surface, NULL, m_screen, &rect);
    SDL_FreeSurface(surface);
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

