/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Landslip.h"

#include "Rules.h"
#include "minmax.h"

#include <string.h>

//-----------------------------------------------------------------
    Landslip::Landslip(const ModelList &models)
: m_models(models)
{
    m_impact = Cube::NONE;
    m_stoned = new bool[m_models.size()];
    memset(m_stoned, false, sizeof(bool) * m_models.size());
}
//-----------------------------------------------------------------
Landslip::~Landslip()
{
    delete[] m_stoned;
}
//-----------------------------------------------------------------
/**
 * Indentify falling objects.
 * @return whether something is falling.
 */
    bool
Landslip::computeFall()
{
    while (m_models.stoneOn(this)) {
        /* empty */
    }
    return m_models.fallOn(this);
}
//-----------------------------------------------------------------
    bool
Landslip::stoneModel(const Cube *model)
{
    bool change = false;
    if (!isStoned(model)) {
        if (isFixed(model) || isOnPad(model)) {
            stone(model);
            change = true;
        }
    }
    return change;
}
//-----------------------------------------------------------------
    bool
Landslip::isOnPad(const Cube *model) const
{
    const Cube::t_models pad = model->const_rules()->getResist(Dir::DIR_DOWN);
    Cube::t_models::const_iterator end = pad.end();
    for (Cube::t_models::const_iterator i = pad.begin(); i != end; ++i) {
        if (isFixed(*i)) {
            return true;
        }
    }
    return false;
}
//-----------------------------------------------------------------
    bool
Landslip::isFixed(const Cube *model) const
{
    return isStoned(model) || model->isWall() ||
        model->isAlive() || model->isLost();
}
//-----------------------------------------------------------------
    bool
Landslip::isStoned(const Cube *model) const
{
    int index = model->getIndex();
    if (index > -1) {
        return m_stoned[index];
    }
    else {
        return true;
    }
}
//-----------------------------------------------------------------
    void
Landslip::stone(const Cube *model)
{
    int index = model->getIndex();
    if (index > -1) {
        m_stoned[index] = true;
    }
}
//-----------------------------------------------------------------
/**
 * Let model to fall.
 * @return true when model is falling
 */
    bool
Landslip::fallModel(Cube *model)
{
    bool falling = false;
    if (!isFixed(model)) {
        model->rules()->actionFall();
        falling = true;
    }
    else {
        bool lastFall = model->rules()->clearLastFall();
        if (lastFall && m_impact < model->getWeight()) {
            m_impact = model->getWeight();
        }
    }
    return falling;
}

