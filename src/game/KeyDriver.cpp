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
#include "Log.h"

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
 */
bool
KeyDriver::drive(Cube *model)
{
    bool result = false;
    if (model->isAlive()) {
        if (false == result && m_keys[m_control.getLeft()]) {
            result = model->moveDir(Cube::DIR_LEFT);
        }
        if (false == result && m_keys[m_control.getRight()]) {
            result = model->moveDir(Cube::DIR_RIGHT);
        }
        if (false == result && m_keys[m_control.getUp()]) {
            result = model->moveDir(Cube::DIR_UP);
        }
        if (false == result && m_keys[m_control.getDown()]) {
            result = model->moveDir(Cube::DIR_DOWN);
        }

        //TODO: draw nice animation
        //TODO: speed up when still the same driver and direction
        //TEST: animation
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

