/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ModelList.h"

#include "View.h"
#include "Landslip.h"

//-----------------------------------------------------------------
/**
 * Create new wrapper.
 * @param models wrapped models.
 */
ModelList::ModelList(const Cube::t_models *models)
{
    m_models = models;
}
//-----------------------------------------------------------------
void
ModelList::drawOn(View *view) const
{
    Cube::t_models::const_iterator end = m_models->end();
    for (Cube::t_models::const_iterator i = m_models->begin(); i != end; ++i) {
        view->drawModel(*i);
    }
}
//-----------------------------------------------------------------
/**
 * Stone all models on fixed pad.
 * @return true when new model was stoned
 */
bool
ModelList::stoneOn(Landslip *slip) const
{
    bool change = false;
    Cube::t_models::const_iterator end = m_models->end();
    for (Cube::t_models::const_iterator i = m_models->begin(); i != end; ++i) {
        if (slip->stoneModel(*i)) {
            change = true;
        }
    }
    return change;
}
//-----------------------------------------------------------------
/**
 * Let all not stoned models to fall.
 * @return true when something is falling
 */
bool
ModelList::fallOn(Landslip *slip) const
{
    bool falling = false;
    Cube::t_models::const_iterator end = m_models->end();
    for (Cube::t_models::const_iterator i = m_models->begin(); i != end; ++i) {
        falling |= slip->fallModel(*i);
    }
    return falling;
}

