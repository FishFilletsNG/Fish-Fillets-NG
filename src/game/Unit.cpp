/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Unit.h"

#include "Log.h"
#include "Cube.h"
#include "Rules.h"

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
Unit::canDrive()
{
    return m_model->isAlive()
        && !m_model->isLost()
        && !m_model->isBusy();
}
//-----------------------------------------------------------------
/**
 * Test keys and try move.
 * @return true when unit has moved
 */
char
Unit::drive(Uint8 *pressed)
{
    return driveBorrowed(pressed, m_buttons);
}
//-----------------------------------------------------------------
/**
 * Test keys and try move, use borrowed controls.
 * @return non zero symbol when unit has moved
 */
char
Unit::driveBorrowed(Uint8 *pressed, const KeyControl &buttons)
{
    if (canDrive()) {
        if (pressed[buttons.getLeft()]) {
            return goLeft();
        }
        if (pressed[buttons.getRight()]) {
            return goRight();
        }
        if (pressed[buttons.getUp()]) {
            return goUp();
        }
        if (pressed[buttons.getDown()]) {
            return goDown();
        }
    }
    return 0;
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
 * Make move.
 * @return move symobol or zero for bad move
 */
char
Unit::driveOrder(char move)
{
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
    return 0;
}
//-----------------------------------------------------------------
char
Unit::goLeft()
{
    char symbol = 0;
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
    char symbol = 0;
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
    char symbol = 0;
    if (m_model->rules()->actionMoveDir(Rules::DIR_UP)) {
        symbol = m_symbols.getUp();
    }
    return symbol;
}
//-----------------------------------------------------------------
char
Unit::goDown()
{
    char symbol = 0;
    if (m_model->rules()->actionMoveDir(Rules::DIR_DOWN)) {
        symbol = m_symbols.getDown();
    }
    return symbol;
}

//-----------------------------------------------------------------
bool
Unit::isMoving()
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
Unit::isTurning()
{
    std::string action = m_model->rules()->getAction();
    return action == "turn";
}
//-----------------------------------------------------------------
bool
Unit::isPushing()
{
    return m_model->rules()->getState() == "pushing";
}


