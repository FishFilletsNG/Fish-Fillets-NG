/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LevelNode.h"

#include "Level.h"
#include "NodeDrawer.h"

#include <math.h>
#include <assert.h>

//-----------------------------------------------------------------
/**
 * Default state is STATE_FAR.
 */
LevelNode::LevelNode(const std::string &codename, const Path &datafile,
                const V2 &loc)
: m_codename(codename), m_datafile(datafile), m_loc(loc)
{
    m_state = STATE_FAR;
}
//-----------------------------------------------------------------
/**
 * Free self and all adjacents.
 */
LevelNode::~LevelNode()
{
    t_adjacents::iterator end = m_adjacents.end();
    for (t_adjacents::iterator i = m_adjacents.begin(); i != end; ++i) {
        delete *i;
    }
}
//-----------------------------------------------------------------
/**
 * Set state for this node and open/hide his followers.
 */
void
LevelNode::setState(eState state)
{
    t_adjacents::iterator end = m_adjacents.end();
    switch (state) {
        case STATE_HIDDEN:
            for (t_adjacents::iterator i = m_adjacents.begin();
                    i != end; ++i)
            {
                (*i)->setState(STATE_HIDDEN);
            }
            break;
        case STATE_FAR:
            break;
        case STATE_OPEN:
            break;
        case STATE_SOLVED:
            for (t_adjacents::iterator i = m_adjacents.begin();
                    i != end; ++i)
            {
                (*i)->setState(STATE_OPEN);
            }
            break;
        default:
            assert(!"unknown level node state");
            break;
    }
    m_state = state;
}
//-----------------------------------------------------------------
/**
 * Returns true when cursor is in radius around node.
 */
bool
LevelNode::isUnder(const V2 &cursor) const
{
    double dx = m_loc.getX() - cursor.getX();
    double dy = m_loc.getY() - cursor.getY();

    return (sqrt(dx * dx + dy * dy) < DOT_RADIUS);
}
//-----------------------------------------------------------------
/**
 * Find selected node under cursor.
 * @return selected node or NULL
 */
LevelNode *
LevelNode::findSelected(const V2 &cursor)
{
    if (m_state >= STATE_OPEN) {
        if (isUnder(cursor)) {
            return this;
        }
        else {
            t_adjacents::const_iterator end = m_adjacents.end();
            for (t_adjacents::const_iterator i = m_adjacents.begin();
                    i != end; ++i)
            {
                LevelNode *selected = (*i)->findSelected(cursor);
                if (selected) {
                    return selected;
                }
            }
        }
    }
    return NULL;
}
//-----------------------------------------------------------------
Level *
LevelNode::createLevel() const
{
    return new Level(m_codename, m_datafile);
}
//-----------------------------------------------------------------
/**
 * Add adjecent.
 * NOTE: cycles in graph are not supported.
 */
void
LevelNode::addAdjacent(LevelNode *node)
{
    m_adjacents.push_back(node);
}
//-----------------------------------------------------------------
void
LevelNode::drawPath(const NodeDrawer *drawer) const
{
    if (m_state > STATE_HIDDEN) {
        t_adjacents::const_iterator end = m_adjacents.end();
        for (t_adjacents::const_iterator i = m_adjacents.begin();
                i != end; ++i)
        {
            if ((*i)->getState() > STATE_HIDDEN) {
                drawer->drawEdge(this, *i);
                (*i)->drawPath(drawer);
            }
        }
        drawer->drawNode(this);
    }
}

