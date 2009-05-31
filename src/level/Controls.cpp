/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Controls.h"

#include "Unit.h"
#include "PhaseLocker.h"

#include "KeyStroke.h"
#include "MouseStroke.h"

//-----------------------------------------------------------------
/**
 * Create list of drivers.
 * @param locker shared locker for anim
 */
Controls::Controls(PhaseLocker *locker)
    : m_units(), m_moves()
{
    m_locker = locker;
    m_active = m_units.begin();
    m_speedup = 0;
    m_switch = true;
    m_strokeSymbol = ControlSym::SYM_NONE;
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
 * Returns active unit or NULL.
 */
const Unit *
Controls::getActive()
{
    Unit *result = NULL;
    if (m_active != m_units.end()) {
        result = *m_active;
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Let drivers to drive.
 * Only one driver can drive at the same time.
 * @param input wrapped input
 * @return true when a fish has moved (switch does not count)
 */
    bool
Controls::driving(const InputProvider *input)
{
    bool moved = false;
    if (!useSwitch()) {
        if (!useStroke()) {
            moved = driveUnit(input);
        }
        else {
            moved = true;
        }
    }
    return moved;
}
//-----------------------------------------------------------------
/**
 * Returns true when a switch was done.
 */
bool
Controls::useSwitch()
{
    bool result = false;
    if (m_active != m_units.end()) {
        if (!(*m_active)->willMove()) {
            checkActive();
        }

        if (m_switch && m_active != m_units.end()) {
            m_locker->ensurePhases(3);
            (*m_active)->activate();
            result = true;
        }
    }
    m_switch = false;
    return result;
}
//-----------------------------------------------------------------
/**
 * Use gathered stroke.
 * NOTE: returns true even for bad move (not used)
 * @return true for used stroke
 */
bool
Controls::useStroke()
{
    bool result = false;
    if (m_strokeSymbol != ControlSym::SYM_NONE) {
       makeMove(m_strokeSymbol);
       m_strokeSymbol = ControlSym::SYM_NONE;
       result = true;
    }
    return result;
}
//-----------------------------------------------------------------
    bool
Controls::driveUnit(const InputProvider *input)
{
    char moved = ControlSym::SYM_NONE;
    if (m_active != m_units.end()) {
        moved = (*m_active)->driveBorrowed(input, m_arrows);
    }

    if (ControlSym::SYM_NONE == moved) {
        t_units::iterator end = m_units.end();
        for (t_units::iterator i = m_units.begin(); i != end; ++i) {
            moved = (*i)->drive(input);
            if (moved != ControlSym::SYM_NONE) {
                setActive(i);
                break;
            }
        }
    }

    if (moved != ControlSym::SYM_NONE) {
        m_moves.append(1, moved);
    }
    return (moved != ControlSym::SYM_NONE);
}
//-----------------------------------------------------------------
    void
Controls::lockPhases()
{
    if (m_active != m_units.end() && (*m_active)->isMoving()) {
        if ((*m_active)->isPushing()) {
            m_speedup = 0;
        }
        else if (!(*m_active)->isTurning()) {
            m_speedup++;
        }

        m_locker->ensurePhases(getNeededPhases(m_speedup));
    }
    else {
        m_speedup = 0;
    }
}
//-----------------------------------------------------------------
int
Controls::getNeededPhases(int speedup) const
{
    static const int SPEED_WARP1 = 6;
    static const int SPEED_WARP2 = 10;

    int phases = 3;
    if (m_active != m_units.end()) {
        if ((*m_active)->isTurning()) {
            phases = (*m_active)->countAnimPhases("turn");
        }
        else if (speedup > SPEED_WARP2) {
            phases = (*m_active)->countAnimPhases("swam") / 6;
        }
        else if (speedup > SPEED_WARP1) {
            phases = (*m_active)->countAnimPhases("swam") / 3;
        }
        else {
            phases = (*m_active)->countAnimPhases("swam") / 2;
        }
    }
    return phases;
}
//-----------------------------------------------------------------
/**
 * Check whether active unit can still drive,
 * otherwise make switch.
 */
void
Controls::checkActive()
{
    if (m_active == m_units.end() || !(*m_active)->canDrive()) {
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
    if (!m_units.empty()) {
        t_units::iterator start = m_active;

        do {
            if (m_active == m_units.end() || m_active + 1 == m_units.end()) {
                m_active = m_units.begin();
            }
            else {
                ++m_active;
            }
        } while (m_active != start && !(*m_active)->canDrive());

        if (start != m_active) {
            m_speedup = 0;
            m_switch = true;
        }
    }
}
//-----------------------------------------------------------------
/**
 * Obtain first control symbol from keyboard events.
 */
void
Controls::controlEvent(const KeyStroke &stroke)
{
    SDLKey key = stroke.getKey();

    if (m_strokeSymbol == ControlSym::SYM_NONE) {
        if (m_active != m_units.end()) {
            m_strokeSymbol = (*m_active)->mySymbolBorrowed(key, m_arrows);
        }

        if (m_strokeSymbol == ControlSym::SYM_NONE) {
            t_units::iterator end = m_units.end();
            for (t_units::iterator i = m_units.begin(); i != end; ++i) {
                m_strokeSymbol = (*i)->mySymbol(key);
                if (m_strokeSymbol != ControlSym::SYM_NONE) {
                    return;
                }
            }
        }
    }
}
//-----------------------------------------------------------------
/**
 * Activate fish under cursor.
 * @param occupant model to activate
 * @return true when fish was selected
 */
bool
Controls::activateSelected(const Cube *occupant)
{
    t_units::iterator end = m_units.end();
    for (t_units::iterator i = m_units.begin(); i != end; ++i) {
        if ((*i)->equalsModel(occupant)) {
            m_active = i;
            m_switch = true;
            return true;
        }
    }
    return false;
}
//-----------------------------------------------------------------
    void
Controls::setMoves(const std::string &moves)
{
    m_moves = moves;
    if (!m_moves.empty()) {
        activateDriven(m_moves[m_moves.size() - 1]);
    }
}
//-----------------------------------------------------------------
/**
 * Activate fish driven by given symbol.
 * @param symbol one of fish symbols
 * @return true when fish was selected
 */
bool
Controls::activateDriven(char symbol)
{
    t_units::iterator end = m_units.end();
    for (t_units::iterator i = m_units.begin(); i != end; ++i) {
        if ((*i)->isDrivenBy(symbol)) {
            m_active = i;
            m_switch = true;
            return true;
        }
    }
    return false;
}
//-----------------------------------------------------------------
/**
 * Change active unit.
 * NOTE: change is without switch animation
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
            m_moves.append(1, move);
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
Controls::cannotMove() const
{
    t_units::const_iterator end = m_units.end();
    for (t_units::const_iterator i = m_units.begin(); i != end; ++i) {
        if ((*i)->willMove()) {
            return false;
        }
    }
    return true;
}
//-----------------------------------------------------------------
/**
 * Returns true when active fish is powerful.
 */
bool
Controls::isPowerful() const
{
    bool result = false;
    if (m_active != m_units.end()) {
        result = (*m_active)->isPowerful();
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Returns true when the active fish is doing a dangerous move.
 */
bool
Controls::isDangerousMove() const
{
    bool result = false;
    if (m_active != m_units.end()) {
        result = (*m_active)->isPushing();
    }
    return result;
}


