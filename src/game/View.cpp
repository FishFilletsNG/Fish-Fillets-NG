/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "View.h"

#include "Cube.h"
#include "Anim.h"

#include "Rules.h"
#include "GameAgent.h"

//-----------------------------------------------------------------
/**
 * Create new view.
 */
View::View()
{
    try {
        m_anim = NULL;
        m_anim = new Anim();

        m_model = NULL;
        m_animShift = 0;
        m_shiftSize = SCALE;
    }
    catch (...) {
        if (m_anim) {
            delete m_anim;
        }
        throw;
    }
}
//-----------------------------------------------------------------
View::~View()
{
    delete m_anim;
}
//-----------------------------------------------------------------
/**
 * Prepare new anim.
 */
    void
View::noteNewRound()
{
    m_animShift = 0;
    computeShiftSize();
}
//-----------------------------------------------------------------
/**
 * Draw model.
 * Care about model shift during move.
 */
    void
View::draw()
{
    if (m_model) {
        V2 loc = m_model->getLocation();

        V2 shift(0, 0);
        Rules::eDir dir = m_model->const_rules()->getDir();
        if (dir != Rules::DIR_NO) {
            ++m_animShift;

            shift = m_model->const_rules()->dir2xy(dir);
            shift = shift.scale(m_animShift * m_shiftSize);
        }

        V2 shift_loc = shift.composition(loc.scale(SCALE));

        Anim::eSide side = Anim::SIDE_LEFT;
        if (false == m_model->isLeft()) {
            side = Anim::SIDE_RIGHT;
        }
        m_anim->drawAt(m_screen, shift_loc.getX(), shift_loc.getY(), side);
    }
}
//-----------------------------------------------------------------
/**
 * Shift size is according the slowest shift.
 */
    void
View::computeShiftSize()
{
    int ensured = GameAgent::agent()->getEnsuredPhases();
    if (ensured > 0) {
        m_shiftSize = SCALE / ensured;
    }
    else {
        m_shiftSize = SCALE;
    }
}

