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
Keymap::registerKey(const std::string &key, const KeyStroke &stroke)
{
    m_keys.insert(std::pair<std::string,KeyStroke>(key, stroke));
}
//-----------------------------------------------------------------
bool
Keymap::isPressed(const std::string &key) const
{
    bool result = false;
    t_keys::const_iterator it = m_keys.find(key);
    if (m_keys.end() != it) {
        result = it->second.isPressed();
    }
    else {
        LOG_WARNING(ExInfo("no such keystroke")
                .addInfo("key", key));
    }

    return result;
}

