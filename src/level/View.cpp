/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "View.h"

#include "PhaseLocker.h"
#include "ModelList.h"

#include "Cube.h"
#include "Anim.h"

#include "Rules.h"

//-----------------------------------------------------------------
/**
 * Create new view.
 * @param locker shared locker for anim
 * @param models wrapper arount models
 */
View::View(const ModelList &models)
     : m_models(models)
{
    m_animShift = 0;
    m_shiftSize = SCALE;
}
//-----------------------------------------------------------------
/**
 * Prepare new anim.
 */
    void
View::noteNewRound(int phases)
{
    m_animShift = 0;
    computeShiftSize(phases);
}
//-----------------------------------------------------------------
void
View::draw()
{
    ++m_animShift;
    m_models.drawOn(this);
}
//-----------------------------------------------------------------
/**
 * Draw model.
 * Care about model shift during move.
 */
    void
View::drawModel(Cube *model)
{
    if (!model->isLost()) {
        V2 loc = model->getLocation();

        V2 shift(0, 0);
        Rules::eDir dir = model->rules()->getDir();
        if (dir != Rules::DIR_NO) {
            shift = Rules::dir2xy(dir);
            shift = shift.scale(m_animShift * m_shiftSize);
        }

        V2 shift_loc = shift.composition(loc.scale(SCALE));

        Anim::eSide side = Anim::SIDE_LEFT;
        if (!model->isLeft()) {
            side = Anim::SIDE_RIGHT;
        }
        model->anim()->drawAt(m_screen,
                shift_loc.getX(), shift_loc.getY(), side);
    }
}
//-----------------------------------------------------------------
/**
 * Split move in a few phases.
 */
    void
View::computeShiftSize(int phases)
{
    if (phases > 0) {
        m_shiftSize = SCALE / phases;
    }
    else {
        m_shiftSize = SCALE;
    }
}

