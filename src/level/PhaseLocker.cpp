/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "PhaseLocker.h"

#include "minmax.h"

//-----------------------------------------------------------------
PhaseLocker::PhaseLocker()
{
    m_lockPhases = 0;
}
//-----------------------------------------------------------------
/**
 * Reserve game cycle for blocking animation.
 * @param count how much phases we need
 */
    void
PhaseLocker::ensurePhases(int count)
{
    m_lockPhases = max(m_lockPhases, count);
}
//-----------------------------------------------------------------
void
PhaseLocker::decLock()
{
    if (m_lockPhases > 0) {
        m_lockPhases--;
    }
}

