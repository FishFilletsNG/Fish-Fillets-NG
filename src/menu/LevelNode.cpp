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
                const V2 &loc, const std::string &poster)
: m_codename(codename), m_poster(poster), m_datafile(datafile), m_loc(loc)
{
    m_state = STATE_FAR;
    m_depth = 1;
    m_bestMoves = -1;
}
//-----------------------------------------------------------------
/**
 * Free self and all children.
 */
LevelNode::~LevelNode()
{
    t_children::iterator end = m_children.end();
    for (t_children::iterator i = m_children.begin(); i != end; ++i) {
        delete *i;
    }
}
//-----------------------------------------------------------------
/**
 * Set state for this node and open his followers.
 */
void
LevelNode::setState(eState state)
{
    t_children::iterator end = m_children.end();
    switch (state) {
        case STATE_HIDDEN:
        case STATE_FAR:
        case STATE_OPEN:
            break;
        case STATE_SOLVED:
            for (t_children::iterator i = m_children.begin();
                    i != end; ++i)
            {
                if ((*i)->getState() < STATE_OPEN) {
                    (*i)->setState(STATE_OPEN);
                }
            }
            break;
        default:
            assert(!"unknown level node state");
            break;
    }
    m_state = state;
}
//-----------------------------------------------------------------
void
LevelNode::bestSolution(int moves, const std::string &author)
{
    m_bestMoves = moves;
    m_bestAuthor = author;
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
 * Only solved and open nodes are traversed.
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
            t_children::const_iterator end = m_children.end();
            for (t_children::const_iterator i = m_children.begin();
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
/**
 * Find next OPEN node.
 * @param current current selected node (could be NULL)
 * @return next OPEN node or NULL
 */
LevelNode *
LevelNode::findNextOpen(const LevelNode *current)
{
    bool beforeCurrent = true;

    t_children opened = findOpenNodes();
    t_children::const_iterator end = opened.end();
    for (t_children::const_iterator i = opened.begin(); i != end; ++i) {
        if (*i == current) {
            beforeCurrent = false;
        }
        else if (!beforeCurrent) {
            return *i;
        }
    }

    if (opened.size() > 0) {
        return opened[0];
    }
    else {
        return NULL;
    }
}
//-----------------------------------------------------------------
/**
 * Find all OPEN child nodes.
 * @return vector of shared pointers
 */
    LevelNode::t_children
LevelNode::findOpenNodes()
{
    t_children opened;
    if (m_state >= STATE_OPEN) {
        if (m_state == STATE_OPEN) {
            opened.push_back(this);
        }

        t_children::const_iterator end = m_children.end();
        for (t_children::const_iterator i = m_children.begin(); i != end; ++i) {
            t_children nodes = (*i)->findOpenNodes();
            opened.insert(opened.end(), nodes.begin(), nodes.end());
        }
    }
    return opened;
}
//-----------------------------------------------------------------
/**
 * Find named node in whole tree.
 * @return named node or NULL
 */
LevelNode *
LevelNode::findNamed(const std::string &codename)
{
    if (m_codename == codename) {
        return this;
    }
    else {
        t_children::const_iterator end = m_children.end();
        for (t_children::const_iterator i = m_children.begin();
                i != end; ++i)
        {
            LevelNode *named = (*i)->findNamed(codename);
            if (named) {
                return named;
            }
        }
    }
    return NULL;
}
//-----------------------------------------------------------------
/**
 * Returns true when all child nodes are solved.
 */
bool
LevelNode::areAllSolved() const
{
    if (m_state != STATE_SOLVED) {
        return false;
    }
    t_children::const_iterator end = m_children.end();
    for (t_children::const_iterator i = m_children.begin(); i != end; ++i) {
        if (!(*i)->areAllSolved()) {
            return false;
        }
    }
    return true;
}
//-----------------------------------------------------------------
Level *
LevelNode::createLevel() const
{
    return new Level(m_codename, m_datafile, m_depth);
}
//-----------------------------------------------------------------
/**
 * Add child node.
 * NOTE: cycles in graph are not supported.
 */
void
LevelNode::addChild(LevelNode *new_node)
{
    m_children.push_back(new_node);

    new_node->setDepth(m_depth + 1);
    if (m_state == STATE_SOLVED && new_node->getState() < STATE_OPEN) {
        new_node->setState(STATE_OPEN);
    }
}
//-----------------------------------------------------------------
/**
 * Draws self and path to all children.
 * Children are drawed recursive.
 */
void
LevelNode::drawPath(const NodeDrawer *drawer) const
{
    if (m_state > STATE_HIDDEN) {
        t_children::const_iterator end = m_children.end();
        for (t_children::const_iterator i = m_children.begin();
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

