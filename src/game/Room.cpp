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
#include "Rules.h"
#include "LogicException.h"
#include "SoundAgent.h"
#include "SubTitleAgent.h"
#include "DialogAgent.h"
#include "Dialog.h"

//-----------------------------------------------------------------
Room::Room(int w, int h, const Path &picture)
{
    m_bg = new Picture(picture, 0, 0);
    m_field = new Field(w, h);
    m_impact = Cube::NONE;

    killDialogs();
}
//-----------------------------------------------------------------
/**
 * Delete field and models.
 */
Room::~Room()
{
    killDialogs();

    Cube::t_models::iterator end = m_models.end();
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        delete (*i);
    }

    delete m_field;
    delete m_bg;
}
//-----------------------------------------------------------------
void
Room::killDialogs()
{
    SubTitleAgent::agent()->removeAll();
    DialogAgent::agent()->removeAll();

    //NOTE: "pause" dialog is used to delay dialogs
    DialogAgent::agent()->addDialog("pause",
            new Dialog(DialogAgent::DEFAULT_LANG, "", ""));
}
//-----------------------------------------------------------------
/**
 * Add model at scene.
 * @return model index
 */
    int
Room::addModel(Cube *model)
{
    model->rules()->takeField(m_field);
    m_models.push_back(model);

    int model_index = m_models.size() - 1;
    model->setIndex(model_index);
    return model_index;
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
 * Return model at location.
 */
Cube *
Room::askField(const V2 &loc)
{
    return m_field->getModel(loc);
}
//-----------------------------------------------------------------
/**
 * Update all models.
 * Prepare new move, let models fall, let models drive, release old position.
 * @return true when room is finished
 */
    bool
Room::nextRound()
{
    prepareRound();

    bool falling = falldown();
    playImpact();

    if (false == falling) {
        driving();
    }

    return finishRound();
}
//-----------------------------------------------------------------
/**
 * Play sound when some object has fall.
 * NOTE: only one sound is played even more objects have fall
 */
void
Room::playImpact()
{
    switch (m_impact) {
        case Cube::NONE:
            break;
        case Cube::LIGHT:
            SoundAgent::agent()->playRandomSound("impact_light");
            break;
        case Cube::HEAVY:
            SoundAgent::agent()->playRandomSound("impact_heavy");
            break;
        default:
            assert("unknown impact weight" == NULL);
    }
    m_impact = Cube::NONE;
}
//-----------------------------------------------------------------
/**
 * Move all models to new position
 * and check dead fihes.
 */
void
Room::prepareRound()
{
    bool interrupt = false;

    //NOTE: we must call this functions sequential for all objects
    Cube::t_models::iterator end = m_models.end();
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        (*i)->rules()->occupyNewPos();
    }
    for (Cube::t_models::iterator j = m_models.begin(); j != end; ++j) {
        interrupt |= (*j)->rules()->checkDead();
    }
    for (Cube::t_models::iterator k = m_models.begin(); k != end; ++k) {
        interrupt |= (*k)->rules()->checkOut();
    }
    for (Cube::t_models::iterator l = m_models.begin(); l != end; ++l) {
        (*l)->rules()->prepareRound();
    }

    if (interrupt) {
        killDialogs();
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
        Rules::eFall fall = (*i)->rules()->actionFall();
        if (Rules::FALL_NOW == fall) {
            result = true;
        }
        else if (Rules::FALL_LAST == fall) {
            if (m_impact < (*i)->getWeight()) {
                m_impact = (*i)->getWeight();
            }
        }
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
//-----------------------------------------------------------------
/**
 * Let models to release their old position.
 * Check complete room.
 * @return true when room is finished
 */
bool
Room::finishRound()
{
    bool room_complete = true;

    Cube::t_models::iterator end = m_models.end();
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        (*i)->finishRound();
        room_complete &= (*i)->isSatisfy();
    }

    return room_complete;
}



