/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WorldMap.h"

#include "LevelNode.h"
#include "NodeDrawer.h"

#include "ResImageAgent.h"
#include "VideoAgent.h"

//-----------------------------------------------------------------
WorldMap::WorldMap(LevelNode *startNode, const Path &bg)
{
    m_bg = ResImageAgent::agent()->loadImage(bg);
    m_selected = NULL;
    m_startNode = startNode;
    m_drawer = new NodeDrawer();
    m_active = true;
}
//-----------------------------------------------------------------
WorldMap::~WorldMap()
{
    SDL_FreeSurface(m_bg);
    delete m_startNode;
    delete m_drawer;
}
//-----------------------------------------------------------------
void
WorldMap::activate()
{
    if (!m_active) {
        VideoAgent::agent()->acceptDrawer(this);
        m_active = true;
    }
}
//-----------------------------------------------------------------
void
WorldMap::deactivate()
{
    m_selected = NULL;
    VideoAgent::agent()->removeDrawer(this);
    m_active = false;
}
//-----------------------------------------------------------------
/**
 * Mark node under cursor as selected.
 */
void
WorldMap::watchCursor()
{
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    m_selected = m_startNode->findSelected(V2(x, y));
}
//-----------------------------------------------------------------
/**
 * Return selected level or NULL;
 */
Level *
WorldMap::createSelected() const
{
    Level *result = NULL;
    if (m_selected) {
        result = m_selected->createLevel();
    }
    return result;
}
//-----------------------------------------------------------------
void
WorldMap::markSolved()
{
    if (m_selected) {
        m_selected->setState(LevelNode::STATE_SOLVED);
    }
}
//-----------------------------------------------------------------
void
WorldMap::draw()
{
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    SDL_BlitSurface(m_bg, NULL, m_screen, &rect);

    m_drawer->setScreen(m_screen);
    m_startNode->drawPath(m_drawer);
    if (m_selected) {
        m_drawer->drawSelected(m_selected);
    }
    //TODO: draw name for node under cursor
}

