/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Room.h"

#include "Log.h"
#include "Picture.h"
#include "Field.h"
#include "LogicException.h"

//-----------------------------------------------------------------
Room::Room(int w, int h, const Path &picture)
{
    m_bg = new Picture(picture, 0, 0);
    m_field = new Field(w, h);
}
//-----------------------------------------------------------------
/**
 * Delete field and models.
 */
Room::~Room()
{
    Cube::t_models::iterator end = m_models.end();
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        delete (*i);
    }

    delete m_field;
    delete m_bg;
}
//-----------------------------------------------------------------
/**
 * Add model at scene.
 * @return model index
 */
    int
Room::addModel(Cube *model)
{
    model->takeField(m_field);
    m_models.push_back(model);
    return m_models.size() - 1;
}
//-----------------------------------------------------------------
/**
 * Return model at index.
 * @throws LogicException when model_index is out of range
 */
Cube *
Room::getModel(int model_index)
{
    Cube *result = NULL;
    if (0 <= model_index && model_index < (int)m_models.size()) {
        result = m_models[model_index];
    }
    else {
        throw LogicException(ExInfo("bad model index")
                .addInfo("model_index", model_index));
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Update all models.
 * Prepare new move, let models fall, let models drive, release old position.
 */
    void
Room::nextRound()
{
    prepareRound();

    if (false == falldown()) {
        driving();
    }

    Cube::t_models::iterator end = m_models.end();
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        (*i)->finishRound();
    }
}
//-----------------------------------------------------------------
/**
 * Move all models to new position
 * and check dead fihes.
 */
void
Room::prepareRound()
{
    //NOTE: we must call this functions sequential for all objects
    Cube::t_models::iterator end = m_models.end();
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        (*i)->occupyNewPos();
    }
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        (*i)->checkDead();
    }
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        (*i)->applyDead();
    }
}
//-----------------------------------------------------------------
/**
 * Let things fall.
 * @return true when something is falling.
 */
    bool
Room::falldown()
{
    bool result = false;
    Cube::t_models::iterator end = m_models.end();
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        result |= (*i)->fall();
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Let drivers to drive.
 * Only one driver can drive at the same time.
 */
    void
Room::driving()
{
    Cube::t_models::iterator end = m_models.end();
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        if ((*i)->drive()) {
            return;
        }
    }
}



