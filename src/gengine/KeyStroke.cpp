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
    m_mod = mod;
}
//-----------------------------------------------------------------
/**
 * KeyStroke comparation.
 * KMOD_NUM|KMOD_CAPS|KMOD_MODE are ignored.
 *
 * @param other other keystroke
 * @return this < other
 */
    bool
KeyStroke::less(const KeyStroke &other) const
{
    static const int KMOD_IGNORE = KMOD_NUM|KMOD_CAPS|KMOD_MODE;

    bool result = m_sym < other.m_sym;
    if (m_sym == other.m_sym) {
        result = (m_mod & ~KMOD_IGNORE) < (other.m_mod & ~KMOD_IGNORE);
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Return text fashion.
 */
std::string
KeyStroke::toString() const
{
    std::string result = StringTool::toString(m_sym);
    result.append("+" + StringTool::toString(m_mod));
    return result;
}

