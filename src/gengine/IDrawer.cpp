/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "IDrawer.h"

#include "VideoAgent.h"

//-----------------------------------------------------------------
/**
 * Activate self.
 */
IDrawer::IDrawer()
{
    m_active = false;
    activate();
}
//-----------------------------------------------------------------
/**
 * Deactivate self.
 */
IDrawer::~IDrawer()
{
    deactivate();
}
//-----------------------------------------------------------------
/**
 * Register to drawers list.
 */
void
IDrawer::activate()
{
    if (!m_active) {
        VideoAgent::agent()->acceptDrawer(this);
        m_active = true;
    }
}
//-----------------------------------------------------------------
/**
 * Remove oneself from drawes list.
 */
void
IDrawer::deactivate()
{
    if (m_active) {
        VideoAgent::agent()->removeDrawer(this);
        m_active = false;
    }
}

