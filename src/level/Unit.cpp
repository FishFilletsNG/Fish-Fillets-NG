/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Unit.h"

#include "Cube.h"
#include "Rules.h"
#include "InputProvider.h"
#include "Anim.h"

//-----------------------------------------------------------------
/**
 * Create new driveable unit.
 * @param buttons control buttons
 * @param symbols move symbols stored in load/save
 * @param a_startActive whether unit should be active at start
 */
Unit::Unit(const KeyControl &buttons, const ControlSym &symbols,
        bool a_startActive)
    : m_buttons(buttons), m_symbols(symbols)
{
    m_model = NULL;
    m_startActive = a_startActive;
}
//-----------------------------------------------------------------
bool
Unit::canDrive() const
{
    return m_model->isAlive()
        && !m_model->isLost()
        && !m_model->isBusy();
}
//-----------------------------------------------------------------
/**
 * Return true when we can move in future.
 */
bool
Unit::willMove() const
{
    return m_model->isAlive()
        && !m_model->isLost();
}
//-----------------------------------------------------------------
/**
 * Test keys and try move.
 * @return a symbol when unit has moved
 */
char
Unit::drive(const InputProvider *input)
{
    return driveBorrowed(input, m_buttons);
}
//-----------------------------------------------------------------
/**
 * Test keys and try move, use borrowed controls.
 * @return a symbol when unit has moved or SYM_NONE
 */
char
Unit::driveBorrowed(const InputProvider *input, const KeyControl &buttons)
{
    if (canDrive()) {
        if (input->isPressed(buttons.getLeft())) {
            return goLeft();
        }
        if (input->isPressed(buttons.getRight())) {
            return goRight();
        }
        if (input->isPressed(buttons.getUp())) {
            return goUp();
        }
        if (input->isPressed(buttons.getDown())) {
            return goDown();
        }
    }
    return ControlSym::SYM_NONE;
}
//-----------------------------------------------------------------
/**
 * Greet the player.
 */
void
Unit::activate()
{
    m_model->rules()->actionActivate();
}
//-----------------------------------------------------------------
/**
 * Translate this key to symbol.
 * @return symbol or SYM_NONE for unknown key
 */
char
Unit::mySymbol(SDLKey key) const
{
    return mySymbolBorrowed(key, m_buttons);
}
//-----------------------------------------------------------------
/**
 * Translate this key to symbol, compare with borrowed buttons.
 * @return symbol or SYM_NONE for unknown key
 */
char
Unit::mySymbolBorrowed(SDLKey key, const KeyControl &buttons) const
{
    if (key == buttons.getLeft()) {
        return m_symbols.getLeft();
    }
    if (key == buttons.getRight()) {
        return m_symbols.getRight();
    }
    if (key == buttons.getUp()) {
        return m_symbols.getUp();
    }
    if (key == buttons.getDown()) {
        return m_symbols.getDown();
    }
    return ControlSym::SYM_NONE;
}
//-----------------------------------------------------------------
/**
 * Make move.
 * @return move symbol or SYM_NONE for bad move
 */
char
Unit::driveOrder(char move)
{
    if (canDrive()) {
        if (m_symbols.getLeft() == move) {
            return goLeft();
        }
        if (m_symbols.getRight() == move) {
            return goRight();
        }
        if (m_symbols.getUp() == move) {
            return goUp();
        }
        if (m_symbols.getDown() == move) {
            return goDown();
        }
    }
    return ControlSym::SYM_NONE;
}
//-----------------------------------------------------------------
char
Unit::goLeft()
{
    char symbol = ControlSym::SYM_NONE;
    if (m_model->isLeft()) {
        if (m_model->rules()->actionMoveDir(Rules::DIR_LEFT)) {
            symbol = m_symbols.getLeft();
        }
    }
    else {
        m_model->rules()->actionTurnSide();
        symbol = m_symbols.getLeft();
    }
    return symbol;
}
//-----------------------------------------------------------------
char
Unit::goRight()
{
    char symbol = ControlSym::SYM_NONE;
    if (!m_model->isLeft()) {
        if (m_model->rules()->actionMoveDir(Rules::DIR_RIGHT)) {
            symbol = m_symbols.getRight();
        }
    }
    else {
        m_model->rules()->actionTurnSide();
        symbol = m_symbols.getRight();
    }
    return symbol;
}
//-----------------------------------------------------------------
char
Unit::goUp()
{
    char symbol = ControlSym::SYM_NONE;
    if (m_model->rules()->actionMoveDir(Rules::DIR_UP)) {
        symbol = m_symbols.getUp();
    }
    return symbol;
}
//-----------------------------------------------------------------
char
Unit::goDown()
{
    char symbol = ControlSym::SYM_NONE;
    if (m_model->rules()->actionMoveDir(Rules::DIR_DOWN)) {
        symbol = m_symbols.getDown();
    }
    return symbol;
}

//-----------------------------------------------------------------
bool
Unit::isMoving() const
{
    bool result = false;
    if (canDrive()) {
        std::string action = m_model->rules()->getAction();
        result = action == "move_left" || action == "move_right"
            || action == "move_up" || action == "move_down"
            || action == "turn";
    }
    return result;
}
//-----------------------------------------------------------------
bool
Unit::isTurning() const
{
    std::string action = m_model->rules()->getAction();
    return action == "turn";
}
//-----------------------------------------------------------------
bool
Unit::isPushing() const
{
    return m_model->rules()->getState() == "pushing";
}
//-----------------------------------------------------------------
int
Unit::countAnimPhases(const std::string &anim) const
{
    return m_model->anim()->countAnimPhases(anim);
}


