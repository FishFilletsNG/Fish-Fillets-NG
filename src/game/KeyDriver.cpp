/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "KeyDriver.h"

#include "InputAgent.h"
#include "GameAgent.h"
#include "Cube.h"

//-----------------------------------------------------------------
KeyDriver::KeyDriver(const KeyControl &control)
    : m_control(control)
{
    m_keys = InputAgent::agent()->getKeys();
    m_lastResult = false;
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
    if (model->isAlive()) {
        if (false == result && m_keys[m_control.getLeft()]) {
            if (model->lookLeft()) {
                result = model->moveDir(Cube::DIR_LEFT);
            }
            else {
                model->turnSide();
                result = true;
            }
        }
        if (false == result && m_keys[m_control.getRight()]) {
            if (false == model->lookLeft()) {
                result = model->moveDir(Cube::DIR_RIGHT);
            }
            else {
                model->turnSide();
                result = true;
            }
        }
        if (false == result && m_keys[m_control.getUp()]) {
            result = model->moveDir(Cube::DIR_UP);
        }
        if (false == result && m_keys[m_control.getDown()]) {
            result = model->moveDir(Cube::DIR_DOWN);
        }

        //TODO: draw nice animation
        //TODO: speed up when the same direction and no pushing
        if (result) {
            int phases = 3;
            if (m_lastResult) {
                phases = 2;
            }
            GameAgent::agent()->ensurePhases(phases);
        }
    }

    m_lastResult = result;
    return result;
}

