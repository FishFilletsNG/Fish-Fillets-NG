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
 * hence InputAgent init must be after VideoAgent and SoundAgent.
 * NOTE: KeyConsole() use Path which asks OptionAgent
 */
    void
InputAgent::own_init()
{
    m_keyBinder = new KeyBinder();
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
                m_keyBinder->keyDown(event.key.keysym);
                if (m_handler) {
                    m_handler->keyEvent(KeyStroke(event.key.keysym));
                }
                break;
            case SDL_KEYUP:
                if (m_handler) {
                    m_handler->keyUp(KeyStroke(event.key.keysym));
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (m_handler) {
                    m_handler->mouseEvent(MouseStroke(event.button));
                }
                break;
            default:
                break;
        }
    }

    if (m_handler) {
        Uint8 buttons;
        V2 mouseLoc = getMouseState(&buttons);
        m_handler->mouseState(mouseLoc, buttons);
    }
}
//-----------------------------------------------------------------
/**
 * Delete console.
 */
    void
InputAgent::own_shutdown()
{
    delete m_keyBinder;
}
//-----------------------------------------------------------------
void
InputAgent::installHandler(InputHandler *handler)
{
    if (m_handler) {
        m_handler->takePressed(NULL);
        m_handler->mouseState(V2(-1, -1), 0);
    }
    m_handler = handler;
    if (m_handler) {
        m_handler->takePressed(m_keys);
        Uint8 buttons;
        V2 mouseLoc = getMouseState(&buttons);
        m_handler->mouseState(mouseLoc, buttons);
    }
}
//-----------------------------------------------------------------
/**
 * Return mouse location.
 * @param out_buttons place where to store state of buttons
 * @return (mouseX, mouseY)
 */
    V2
InputAgent::getMouseState(Uint8 *out_buttons)
{
    int x;
    int y;
    Uint8 pressed = SDL_GetMouseState(&x, &y);
    if (out_buttons) {
        *out_buttons = pressed;
    }
    return V2(x, y);
}


