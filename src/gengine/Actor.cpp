/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Actor.h"

#include "ExInfo.h"

//-----------------------------------------------------------------
/**
 * Actor is not busy at start.
 */
Actor::Actor()
{
    m_busy = false;
    m_index = -1;
}
//-----------------------------------------------------------------
bool
Actor::equals(const Actor *other) const
{
    return m_index == other->m_index;
}
//-----------------------------------------------------------------
std::string
Actor::toString() const
{
    return ExInfo("actor")
        .addInfo("index", m_index).what();
}


