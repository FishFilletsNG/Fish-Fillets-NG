/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Keymap.h"

#include "Log.h"

//-----------------------------------------------------------------
void
Keymap::registerKey(const KeyStroke &stroke, const KeyDesc &desc)
{
    m_keys.insert(std::pair<KeyStroke,KeyDesc>(stroke, desc));
}
//-----------------------------------------------------------------
/**
 * Return index of pressed key.
 * @return index or -1
 */
int
Keymap::indexPressed(const KeyStroke &stroke) const
{
    int result = -1;
    t_keys::const_iterator it = m_keys.find(stroke);
    if (m_keys.end() != it) {
        result = it->second.getIndex();
    }
    return result;
}

