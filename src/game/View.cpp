/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "View.h"

#include "Anim.h"
#include "Cube.h"
#include "GameAgent.h"

//-----------------------------------------------------------------
/**
 * Create new view.
 */
View::View(const Path &picture)
{
    try {
        m_anim = NULL;
        m_anim = new Anim();
        m_anim->addAnim("default", picture);
        m_anim->setAnim("default");

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
View::prepareRound()
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

        int shiftX = 0;
        int shiftY = 0;
        Cube::eDir dir = m_model->getDir();
        if (dir != Cube::DIR_NO) {
            ++m_animShift;

            m_model->dir2xy(dir, &shiftX, &shiftY);
            shiftX *= m_animShift * m_shiftSize;
            shiftY *= m_animShift * m_shiftSize;
        }

        int x = loc.getX() * SCALE + shiftX;
        int y = loc.getY() * SCALE + shiftY;

        Anim::eSide side = Anim::SIDE_LEFT;
        if (false == m_model->lookLeft()) {
            side = Anim::SIDE_RIGHT;
        }
        m_anim->drawAt(m_screen, x, y, side);
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

