/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Cube.h"

#include "Log.h"
#include "View.h"
#include "Shape.h"
#include "Driver.h"
#include "Rules.h"
#include "ResSoundAgent.h"
#include "SoundAgent.h"
#include "LayoutException.h"
#include "DialogAgent.h"

#include <assert.h>

//-----------------------------------------------------------------
/**
 * Create new model.
 */
Cube::Cube(const V2 &location,
        eWeight weight, eWeight power, bool alive,
        Driver *driver, View *a_view, Shape *shape)
: m_loc(location), m_goal(Goal::noGoal())
{
    m_weight = weight;
    m_power = power;
    m_alive = alive;
    m_out = false;
    m_lookLeft = true;
    m_index = -1;

    m_shape = shape;
    m_driver = driver;
    m_view = a_view;
    m_view->takeModel(this);
    m_rules = new Rules(this);

    //TODO: who will remove this resouces?
    ResSoundAgent *ressound = ResSoundAgent::agent();
    if (0 == ressound->countRes("xplo")) {
        ressound->addSound("xplo", Path::dataReadPath("sound/xplo_01.wav"));
        ressound->addSound("xplo", Path::dataReadPath("sound/xplo_02.wav"));
        ressound->addSound("xplo", Path::dataReadPath("sound/xplo_03.wav"));
        ressound->addSound("xplo", Path::dataReadPath("sound/xplo_04.wav"));
    }
}
//-----------------------------------------------------------------
/**
 * Delete all member but not field.
 */
Cube::~Cube()
{
    //NOTE: rules must be destroyed before shape
    delete m_rules;

    delete m_driver;
    delete m_view;
    delete m_shape;
}

//-----------------------------------------------------------------
/**
 * Make skeleton.
 */
    void
Cube::change_die()
{
    //TODO: nice dead
    LOG_INFO(ExInfo("dead")
            .addInfo("fish", toString()));
    m_alive = false;
    DialogAgent::agent()->killSound(m_index);
    SoundAgent::agent()->playRandomSound("xplo");
}
//-----------------------------------------------------------------
/**
 * Go out of room.
 */
    void
Cube::change_goOut()
{
    //TODO: nice anim
    LOG_INFO(ExInfo("out of room")
            .addInfo("object", toString()));
    //FIXME: where to move outher object
    m_loc = V2(-1000, -1000);
    m_out = true;
}
//-----------------------------------------------------------------
    void
Cube::change_turnSide()
{
    m_lookLeft = !m_lookLeft;
}

//-----------------------------------------------------------------
/**
 * Let the driver drive.
 * Return true when we have moved.
 */
    bool
Cube::drive()
{
    return m_driver->drive(this);
}
//-----------------------------------------------------------------
/**
 * Unmask from old position.
 * And prepare do draw.
 */
    void
Cube::finishRound()
{
    m_rules->freeOldPos();
    m_view->noteNewRound();
}
//-----------------------------------------------------------------
    Anim *
Cube::anim()
{
    return m_view->anim();
}
//-----------------------------------------------------------------
std::string
Cube::toString() const
{
    return std::string(ExInfo("model")
            .addInfo("loc", m_loc.toString())
            .addInfo("alive", m_alive)
            .addInfo("weight", m_weight)
            .addInfo("power", m_power)
            .addInfo("shape", m_shape->toString()).what());
}


