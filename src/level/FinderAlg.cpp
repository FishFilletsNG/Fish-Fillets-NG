/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "FinderAlg.h"

#include "V2.h"
#include "Unit.h"
#include "FinderPlace.h"

//-----------------------------------------------------------------
FinderAlg::FinderAlg(int w, int h)
    : m_closed(w, h)
{
    m_unit = NULL;
}
//-----------------------------------------------------------------
/**
 * Finds the best start direction to the destination.
 * @param unit unit which finds path
 * @param dest destination, final destination must be under unit
 * @return direction or DIR_NO
 */
Dir::eDir
FinderAlg::findDir(const Unit *unit, const V2 &dest)
{
    if (m_unit) {
        m_closed.reset();
        m_fifo.clear();
    }
    m_unit = unit;
    V2 uLoc = unit->getLoc();
    int w = unit->getW();
    int h = unit->getH();
    if (isInRect(uLoc, w, h, dest)) {
        return Dir::DIR_NO;
    }
    //TODO: don't compute when dest is on the wall

    m_closed.markClosed(uLoc);
    m_fifo.push_back(FinderPlace(Dir::DIR_LEFT, uLoc.plus(V2(-1, 0))));
    m_fifo.push_back(FinderPlace(Dir::DIR_RIGHT, uLoc.plus(V2(+1, 0))));
    m_fifo.push_back(FinderPlace(Dir::DIR_UP, uLoc.plus(V2(0, -1))));
    m_fifo.push_back(FinderPlace(Dir::DIR_DOWN, uLoc.plus(V2(0, +1))));

    while (!m_fifo.empty()) {
        FinderPlace place = m_fifo.front();
        m_fifo.pop_front();
        if (tryPlace(place)) {
            if (isInRect(place.getLoc(), w, h, dest)) {
                return place.getStartDir();
            }

            pushNext(place, V2(-1, 0));
            pushNext(place, V2(+1, 0));
            pushNext(place, V2(0, -1));
            pushNext(place, V2(0, +1));
        }
    }
    return Dir::DIR_NO;
}
//-----------------------------------------------------------------
/**
 * Push neighbour to the fifo.
 * Only open place is stored.
 */
void
FinderAlg::pushNext(const FinderPlace &parent, const V2 &shift)
{
    V2 loc = parent.getLoc().plus(shift);
    if (!m_closed.isClosed(loc)) {
        m_closed.markClosed(loc);
        m_fifo.push_back(FinderPlace(parent.getStartDir(), loc));
    }
}
//-----------------------------------------------------------------
/**
 * Whether dest in inside given rectangle.
 */
bool
FinderAlg::isInRect(const V2 &rectLoc, int w, int h, const V2 &dest) const
{
    int rX = rectLoc.getX();
    int rY = rectLoc.getY();
    int destX = dest.getX();
    int destY = dest.getY();
    return (rX <= destX && destX < rX + w)
        && (rY <= destY && destY < rY + h);
}
//-----------------------------------------------------------------
bool
FinderAlg::tryPlace(const FinderPlace &place) const
{
    return m_unit->isFreePlace(place.getLoc());
}

