/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "KeyDriver.h"

#include "Log.h"
#include "InputAgent.h"
#include "GameAgent.h"
#include "Cube.h"
#include "Rules.h"

//-----------------------------------------------------------------
KeyDriver::KeyDriver(const KeyControl &control)
    : m_control(control)
{
    m_keys = InputAgent::agent()->getKeys();
    m_speedup = 0;
}
//-----------------------------------------------------------------
/**
 * Alive models are drived by player.
 * NOTE: it is allowed to move only in one direction at the same time
 *
 * @return whether we have moved
 */
bool
KeyDriver::drive(Cube *model)
{
    bool result = false;
    if (model->isAlive() && false == model->isOut()) {
        if (false == result && m_keys[m_control.getLeft()]) {
            if (model->isLeft()) {
                result = model->rules()->actionMoveDir(Rules::DIR_LEFT);
            }
            else {
                model->rules()->actionTurnSide();
                result = true;
            }
        }
        if (false == result && m_keys[m_control.getRight()]) {
            if (false == model->isLeft()) {
                result = model->rules()->actionMoveDir(Rules::DIR_RIGHT);
            }
            else {
                model->rules()->actionTurnSide();
                result = true;
            }
        }
        if (false == result && m_keys[m_control.getUp()]) {
            result = model->rules()->actionMoveDir(Rules::DIR_UP);
        }
        if (false == result && m_keys[m_control.getDown()]) {
            result = model->rules()->actionMoveDir(Rules::DIR_DOWN);
        }

        speedup(model);
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Speed up fish move for long trace and fish is no pushing.
 * @param moved whether fish has moved
 */
void
KeyDriver::speedup(const Cube *model)
{
    //NOTE: original limits were {6, 10}
    static const int SPEED_WARP1 = 3;
    static const int SPEED_WARP2 = 10;

    int phases = 3;
    std::string action = model->const_rules()->getAction();
    if (action == "move_left" || action == "move_right"
            || action == "move_up" || action == "move_down")
    {
        if (model->const_rules()->getState() == "pushing") {
            m_speedup = 0;
        }
        else {
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
    else if (action == "turn") {
        GameAgent::agent()->ensurePhases(phases);
    }
    else {
        m_speedup = 0;
    }
}

