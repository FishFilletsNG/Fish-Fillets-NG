/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Controls.h"

#include "Log.h"
#include "Unit.h"
#include "InputAgent.h"
#include "GameAgent.h"

//-----------------------------------------------------------------
Controls::Controls()
    : m_units(), m_moves()
{
    m_active = m_units.begin();
    m_speedup = 0;
    m_pressed = InputAgent::agent()->getKeys();
    m_switch = true;
}
//-----------------------------------------------------------------
/**
 * Delete drivers.
 */
Controls::~Controls()
{
    t_units::iterator end = m_units.end();
    for (t_units::iterator i = m_units.begin(); i != end; ++i) {
        delete (*i);
    }
}
//-----------------------------------------------------------------
/**
 * Add unit under our control.
 * @return model index
 */
    void
Controls::addUnit(Unit *unit)
{
    m_units.push_back(unit);
    //NOTE: insertion invalidates m_active
    t_units::iterator end = m_units.end();
    for (t_units::iterator i = m_units.begin(); i != end; ++i) {
        if ((*i)->startActive()) {
            setActive(i);
            return;
        }
    }
    setActive(m_units.begin());
}
//-----------------------------------------------------------------
/**
 * Let drivers to drive.
 * Only one driver can drive at the same time.
 */
    void
Controls::driving()
{
    if (!finishSwitch()) {
        driveUnit();
    }
}
//-----------------------------------------------------------------
/**
 * Returns true when a switch was done.
 */
bool
Controls::finishSwitch()
{
    bool result = false;
    if (m_switch && m_active != m_units.end()) {
        GameAgent::agent()->ensurePhases(3);
        (*m_active)->activate();
        result = true;
    }
    m_switch = false;
    return result;
}
//-----------------------------------------------------------------
void
Controls::driveUnit()
{
    char moved = 0;
    if (m_active != m_units.end()) {
        moved = (*m_active)->driveBorrowed(m_pressed, m_arrows);
    }

    if (0 == moved) {
        t_units::iterator end = m_units.end();
        for (t_units::iterator i = m_units.begin(); i != end; ++i) {
            moved = (*i)->drive(m_pressed);
            if (moved > 0) {
                setActive(i);
                break;
            }
        }
    }

    if (moved > 0) {
        m_moves.push_back(moved);
    }
}
//-----------------------------------------------------------------
    void
Controls::lockPhases()
{
    //NOTE: original limits were {6, 10}
    static const int SPEED_WARP1 = 6;
    static const int SPEED_WARP2 = 10;

    if (m_active != m_units.end() && (*m_active)->isMoving()) {
        int phases = 3;
        if ((*m_active)->isPushing()) {
            m_speedup = 0;
        }
        else if (!(*m_active)->isTurning()) {
            m_speedup++;
        }

        if (m_speedup > SPEED_WARP2) {
            phases = 1;
        }
        else if (m_speedup > SPEED_WARP1) {
            phases = 2;
        }
        else {
            phases = 3;
        }
        GameAgent::agent()->ensurePhases(phases);
    }
    else {
        m_speedup = 0;
    }
}
//-----------------------------------------------------------------
/**
 * Check whether active unit can still drive,
 * otherwise make switch.
 */
void
Controls::checkActive()
{
    if (!(*m_active)->canDrive()) {
        switchActive();
    }
}
//-----------------------------------------------------------------
/**
 * Switch active unit.
 * Activate next driveable unit.
 */
void
Controls::switchActive()
{
    t_units::iterator start = m_active;

    do {
        if (m_active == m_units.end() || m_active + 1 == m_units.end()) {
            m_active = m_units.begin();
        }
        else {
            ++m_active;
        }
    } while (!(*m_active)->canDrive() && m_active != start);

    if (start != m_active) {
        m_speedup = 0;
        m_switch = true;
    }
}
//-----------------------------------------------------------------
/**
 * Change active unit.
 */
void
Controls::setActive(t_units::iterator active)
{
    if (m_active != active) {
        m_speedup = 0;
        m_active = active;
    }
}
//-----------------------------------------------------------------
/**
 * Make this move.
 * @return false for bad move
 */
bool
Controls::makeMove(char move)
{
    t_units::iterator end = m_units.end();
    for (t_units::iterator i = m_units.begin(); i != end; ++i) {
        if ((*i)->driveOrder(move) == move) {
            setActive(i);
            m_moves.push_back(move);
            return true;
        }
    }
    return false;
}
//-----------------------------------------------------------------
/**
 * Returns true when there is no unit which will be able to move.
 */
bool
Controls::cannotMove()
{
    t_units::iterator end = m_units.end();
    for (t_units::iterator i = m_units.begin(); i != end; ++i) {
        if ((*i)->willMove()) {
            return false;
        }
    }
    return true;
}


