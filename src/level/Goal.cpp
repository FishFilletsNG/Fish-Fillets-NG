/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Goal.h"

#include "Cube.h"

//-----------------------------------------------------------------
Goal::Goal(eSATISFY out, eSATISFY alive)
{
    m_out = out;
    m_alive = alive;
}
//-----------------------------------------------------------------
    Goal
Goal::noGoal()
{
    return Goal(SATISFY_IGNORE, SATISFY_IGNORE);
}
//-----------------------------------------------------------------
    Goal
Goal::outGoal()
{
    return Goal(SATISFY_TRUE, SATISFY_IGNORE);
}
//-----------------------------------------------------------------
    Goal
Goal::escapeGoal()
{
    return Goal(SATISFY_TRUE, SATISFY_TRUE);
}
//-----------------------------------------------------------------
    Goal
Goal::aliveGoal()
{
    return Goal(SATISFY_IGNORE, SATISFY_TRUE);
}
//-----------------------------------------------------------------
bool
Goal::isSatisfy(const Cube *model) const
{
    bool result = true;
    if (SATISFY_TRUE == m_out) {
        result &= model->isOut(); 
    }
    else if (SATISFY_FALSE == m_out) {
        result &= (!model->isOut());
    }

    if (SATISFY_TRUE == m_alive) {
        result &= model->isAlive();
    }
    else if (SATISFY_FALSE == m_alive) {
        result &= (!model->isAlive());
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Return whether goal cannot by solved any more.
 * Dead fish cannot be revived.
 * Object out of room cannot go back.
 */
bool
Goal::isWrong(const Cube *model) const
{
    bool wrong = false;
    if (SATISFY_TRUE == m_alive) {
        wrong |= !model->isAlive();
    }

    if (SATISFY_FALSE == m_out) {
        wrong |= model->isOut();
    }
    return wrong;
}
//-----------------------------------------------------------------
bool
Goal::shouldGoOut() const
{
    return SATISFY_TRUE == m_out;
}


