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
#include "ConsoleInput.h"

#include "Log.h"
#include "Name.h"
#include "StringMsg.h"
#include "MessagerAgent.h"
#include "BaseException.h"
#include "Path.h"

//-----------------------------------------------------------------
/**
 * Console starts as deactivated.
 */
    KeyConsole::KeyConsole()
:  m_color(0, 200, 0)
{
    m_font = new Font(Path::dataReadPath("font/font_console.ttf"), 16);
    takeHandler(new ConsoleInput(this));
    registerDrawable(this);
}
//-----------------------------------------------------------------
KeyConsole::~KeyConsole()
{
    delete m_font;
}
//-----------------------------------------------------------------
/**
 * Send console input to scriptAgent.
 * NOTE: debug script fail is not critical
 * @return true for success
 */
bool
KeyConsole::sendCommand()
{
    bool result = false;
    try {
        StringMsg *msg = new StringMsg(Name::SCRIPT_NAME,
                "dostring", m_input);
        MessagerAgent::agent()->forwardNewMsg(msg);
        result = true;
    }
    catch (BaseException &e) {
        LOG_WARNING(e.info());
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Draw console.
 */
    void
KeyConsole::drawOn(SDL_Surface *screen)
{
    SDL_Rect rect;
    rect.x = 10;
    rect.y = 10;

    SDL_Surface *surface = m_font->renderText("console] " + m_input, m_color);
    SDL_BlitSurface(surface, NULL, screen, &rect);
    SDL_FreeSurface(surface);
}

