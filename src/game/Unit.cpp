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
Unit::Unit(const KeyControl &buttons, const ControlSym &symbols)
    : m_buttons(buttons), m_symbols(symbols)
{
    m_model = NULL;
}
//-----------------------------------------------------------------
bool
Unit::canDrive()
{
    return m_model->isAlive()
        && false == m_model->isOut()
        && false == m_model->isBusy();
}
//-----------------------------------------------------------------
bool
Unit::canMove()
{
    return canDrive()
        && false == m_model->isBusy();
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
    if (canMove()) {
        if (pressed[buttons.getLeft()]) {
            if (m_model->isLeft()) {
                if (m_model->rules()->actionMoveDir(Rules::DIR_LEFT)) {
                    return m_symbols.getLeft();
                }
            }
            else {
                m_model->rules()->actionTurnSide();
                return m_symbols.getLeft();
            }
        }
        if (pressed[buttons.getRight()]) {
            if (false == m_model->isLeft()) {
                if (m_model->rules()->actionMoveDir(Rules::DIR_RIGHT)) {
                    return m_symbols.getRight();
                }
            }
            else {
                m_model->rules()->actionTurnSide();
                return m_symbols.getRight();
            }
        }
        if (pressed[buttons.getUp()]) {
            if (m_model->rules()->actionMoveDir(Rules::DIR_UP)) {
                return m_symbols.getUp();
            }
        }
        if (pressed[buttons.getDown()]) {
            if (m_model->rules()->actionMoveDir(Rules::DIR_DOWN)) {
                return m_symbols.getDown();
            }
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


