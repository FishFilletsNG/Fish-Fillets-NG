/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "MultiDrawer.h"

//-----------------------------------------------------------------
/**
 * Store drawer at the end of list.
 */
    void
MultiDrawer::acceptDrawer(Drawable *drawer)
{
    m_drawers.push_back(drawer);
}
//-----------------------------------------------------------------
/**
 * Remove equal drawer from list.
 * Drawer will not be deleted.
 */
    void
MultiDrawer::removeDrawer(const Drawable *drawer)
{
    t_drawers::iterator end = m_drawers.end();
    for (t_drawers::iterator i = m_drawers.begin(); i != end; ++i) {
        if (*i == drawer) {
            m_drawers.erase(i);
            return;
        }
    }
}
//-----------------------------------------------------------------
/**
 * Remove all drawers from list.
 */
void
MultiDrawer::removeAll()
{
    m_drawers.clear();
}
//-----------------------------------------------------------------
/**
 * Let every registred drawer to draw.
 */
void
MultiDrawer::drawOn(SDL_Surface *screen)
{
    t_drawers::iterator end = m_drawers.end();
    for (t_drawers::iterator i = m_drawers.begin(); i != end; ++i) {
        (*i)->drawOn(screen);
    }
}

