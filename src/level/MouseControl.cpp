/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "MouseControl.h"

#include "Controls.h"
#include "View.h"
#include "FinderAlg.h"
#include "Unit.h"
#include "InputProvider.h"

//-----------------------------------------------------------------
MouseControl::MouseControl(Controls *controls, const View *view,
        FinderAlg *finder)
{
    m_controls = controls;
    m_view = view;
    m_finder = finder;
}
//-----------------------------------------------------------------
/**
 * Left button finds shortest path.
 * Right button moves to the cursor.
 * @return whether a move was made
 */
bool
MouseControl::mouseDrive(const InputProvider *input) const
{
    bool moved = false;
    V2 field = m_view->getFieldPos(input->getMouseLoc());
    if (input->isLeftPressed()) {
        moved = moveTo(field);
    }
    else if (input->isRightPressed()) {
        moved = moveHardTo(field);
    }
    return moved;
}
//-----------------------------------------------------------------
/**
 * Move along shortest path without pushing.
 * @param field destination field
 * @return whether a move was made
 */
bool
MouseControl::moveTo(const V2 &field) const
{
    bool moved = false;
    const Unit *unit = m_controls->getActive();
    if (unit) {
        Dir::eDir dir = m_finder->findDir(unit, field);
        if (dir != Dir::DIR_NO) {
             moved = m_controls->makeMove(unit->myOrder(dir));
        }
    }
    return moved;
}
//-----------------------------------------------------------------
/**
 * Move direct to the destination.
 * @param field destination field
 * @return whether a move was made
 */
bool
MouseControl::moveHardTo(const V2 &field) const
{
    bool moved = false;
    const Unit *unit = m_controls->getActive();
    if (unit) {
        V2 loc = unit->getLoc();
        if (field.getX() < loc.getX()) {
            moved = m_controls->makeMove(unit->myOrder(Dir::DIR_LEFT));
        }
        else if (loc.getX() + unit->getW() <= field.getX()) {
            moved = m_controls->makeMove(unit->myOrder(Dir::DIR_RIGHT));
        }
        else if (field.getY() < loc.getY()) {
            moved = m_controls->makeMove(unit->myOrder(Dir::DIR_UP));
        }
        else if (loc.getY() + unit->getH() <= field.getY()) {
            moved = m_controls->makeMove(unit->myOrder(Dir::DIR_DOWN));
        }
    }
    return moved;
}

