/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "InputAgent.h"

#include "KeyBinder.h"
#include "RectBinder.h"
#include "IKeyConsole.h"
#include "InputHandler.h"

#include "MessagerAgent.h"
#include "SimpleMsg.h"
#include "UnknownMsgException.h"
#include "Name.h"
#include "MouseStroke.h"

#include "SDL.h"

//-----------------------------------------------------------------
/**
 * Enable SDL_UNICODE.
 * Enable key repeat.
 * Set console handler to ScriptAgent.
 *
 * NOTE: every SDL_InitSubSystem will disable UNICODE
 * hence InputAgent init must be after VideoAgent.
 * NOTE: KeyConsole() use Path which asks OptionAgent
 */
    void
InputAgent::own_init()
{
    m_keyBinder = new KeyBinder();
    m_rectBinder = new RectBinder();
    m_console = NULL;
    m_handler = NULL;
    m_keys = SDL_GetKeyState(NULL);

    SDL_EnableUNICODE(1);
}
//-----------------------------------------------------------------
    void
InputAgent::own_update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                {
                    BaseMsg *msg = new SimpleMsg(Name::APP_NAME, "quit");
                    MessagerAgent::agent()->forwardNewMsg(msg);
                    break;
                }
            case SDL_KEYDOWN:
                if (m_console && m_console->isActive()) {
                    m_console->keyDown(event.key.keysym);
                }
                else {
                    m_keyBinder->keyDown(event.key.keysym);
                    if (m_handler) {
                        m_handler->keyEvent(KeyStroke(event.key.keysym));
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (SDL_BUTTON_LEFT == event.button.button) {
                    m_rectBinder->lbuttonDown(event.button);
                }
                if (m_handler) {
                    m_handler->mouseEvent(MouseStroke(event.button));
                }
                break;
            default:
                break;
        }
    }
}
//-----------------------------------------------------------------
/**
 * Delete console.
 */
    void
InputAgent::own_shutdown()
{
    if (m_console) {
        delete m_console;
    }
    delete m_rectBinder;
    delete m_keyBinder;
}
//-----------------------------------------------------------------
void 
InputAgent::enableConsole(IKeyConsole *new_console)
{
    if (m_console) {
        delete m_console;
    }
    m_console = new_console;
}
//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - console ... activate debug console
 *
 * @throws UnknownMsgException
 */
void
InputAgent::receiveSimple(const SimpleMsg *msg)
{
    if (msg->equalsName("console")) {
        if (m_console) {
            m_console->activate();
        }
    }
    else {
        throw UnknownMsgException(msg);
    }
}


