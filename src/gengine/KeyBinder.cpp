/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "KeyBinder.h"

#include "Log.h"
#include "LogicException.h"
#include "BaseMsg.h"

#include "SDL.h"

//-----------------------------------------------------------------
KeyBinder::~KeyBinder()
{
    t_strokes::iterator end = m_strokes.end();
    for (t_strokes::iterator i = m_strokes.begin(); i != end; ++i) {
        delete i->second;
    }
}
//-----------------------------------------------------------------
/**
 * Bind keystroke.
 * @param stroke keystroke
 * @param msg message to raise, will be deleted
 * @throws LogicException when keystroke is occupied
 */
void
KeyBinder::addStroke(const KeyStroke &stroke, BaseMsg *msg)
{
    std::pair<t_strokes::iterator,bool> status =
        m_strokes.insert(
                std::pair<KeyStroke,BaseMsg*>(stroke, msg));
    if (!status.second) {
        throw LogicException(ExInfo("keystroke is occupied")
                .addInfo("keystroke", stroke.toString()));
    }
    else {
        LOG_DEBUG(ExInfo("binding keystroke")
                .addInfo("keystroke", stroke.toString())
                .addInfo("msg", msg->toString()));
    }
}
//-----------------------------------------------------------------
void
KeyBinder::removeStroke(const KeyStroke &stroke)
{
    t_strokes::iterator it = m_strokes.find(stroke);
    if (m_strokes.end() != it) {
        delete it->second;
        m_strokes.erase(it);
    }
    else {
        LOG_WARNING(ExInfo("keystroke does not exist")
                .addInfo("keystroke", stroke.toString()));
    }
}
//-----------------------------------------------------------------
/**
 * Handle keydown event,
 * find keystroke and send message.
 */
void
KeyBinder::keyDown(const SDL_keysym &keysym) const
{
    KeyStroke stroke(keysym);
    t_strokes::const_iterator it = m_strokes.find(stroke);
    if (m_strokes.end() != it) {
        it->second->sendClone();
    }
}

