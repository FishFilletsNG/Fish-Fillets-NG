/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "View.h"

#include "Log.h"
#include "ResImageAgent.h"
#include "GameAgent.h"
#include "Cube.h"

#include <assert.h>

//-----------------------------------------------------------------
/**
 * Create new view.
 */
View::View(const Path &picture)
{
    try {
        //NOTE: hack ResImageAgent as sprites holder
        m_anim = new ResImageAgent();
        m_anim->addImage("default", picture);
        setAnim("default");

        m_model = NULL;
        m_animShift = 0;
        m_shiftSize = SCALE;
    }
    catch (...) {
        delete m_anim;
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
            if (0 == m_animShift) {
                computeShiftSize();
            }
            ++m_animShift;

            m_model->dir2xy(dir, &shiftX, &shiftY);
            shiftX *= m_animShift * m_shiftSize;
            shiftY *= m_animShift * m_shiftSize;
        }

        int x = loc.getX() * SCALE + shiftX;
        int y = loc.getY() * SCALE + shiftY;

        drawAt(x, y);
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
//-----------------------------------------------------------------
/**
 * Draw model at screen position.
 */
    void
View::drawAt(int x, int y)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;

    SDL_BlitSurface(m_surface, NULL, m_screen, &rect);
}
//-----------------------------------------------------------------
/**
 * Add picture to anim.
 */
    void
View::addAnim(const std::string &name, const Path &picture)
{
    m_anim->addImage(name, picture);
}
//-----------------------------------------------------------------
/**
 * Set visage.
 * Set m_surface to actual animation picture.
 */
    void
View::setAnim(const std::string &name, int phase)
{
    m_surface = m_anim->getRes(name, phase);
}

