/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "KeyStroke.h"

#include "StringTool.h"

//-----------------------------------------------------------------
/**
 * Create new keystroke from event.
 */
KeyStroke::KeyStroke(const SDL_keysym &keysym)
{
    m_sym = keysym.sym;
    m_mod = modStrip(keysym.mod);
    m_unicode = keysym.unicode;
}
//-----------------------------------------------------------------
/**
 * Create new keystroke.
 * NOTE: KMOD_ALT mean (KMOD_LALT and KMOD_RALT),
 * i.e. either ALTs pressed!
 *
 * @param sym SDLKey
 * @param mod SDLMod ored
 */
KeyStroke::KeyStroke(SDLKey sym, int mod)
{
    m_sym = sym;
    m_mod = modStrip(mod);
    m_unicode = 0;
}
//-----------------------------------------------------------------
/**
 * Strip ignored modes.
 * KMOD_SHIFT|KMOD_NUM|KMOD_CAPS|KMOD_MODE are ignored.
 */
    int
KeyStroke::modStrip(int mod)
{
    return mod & ~STROKE_IGNORE;
}
//-----------------------------------------------------------------
/**
 * KeyStroke comparation.
 *
 * @param other other keystroke
 * @return this < other
 */
bool
KeyStroke::less(const KeyStroke &other) const
{
    bool result = m_sym < other.m_sym;
    if (m_sym == other.m_sym) {
        result = m_mod < other.m_mod;
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Test keyStroke equality.
 * KMOD_NUM|KMOD_CAPS|KMOD_MODE are ignored.
 *
 * @param other other keystroke
 * @return this == other
 */
bool
KeyStroke::equals(const KeyStroke &other) const
{
    return m_sym == other.m_sym &&
        m_mod == other.m_mod;
}
//-----------------------------------------------------------------
/**
 * Return text fashion.
 */
std::string
KeyStroke::toString() const
{
    std::string result = SDL_GetKeyName(m_sym);
    result.append("+" + StringTool::toString(m_mod));
    return result;
}

