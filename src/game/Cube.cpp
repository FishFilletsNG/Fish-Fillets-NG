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
#include "Rules.h"
#include "LayoutException.h"
#include "Anim.h"

//-----------------------------------------------------------------
/**
 * Create new model.
 */
Cube::Cube(const V2 &location,
        eWeight weight, eWeight power, bool alive,
        View *a_view, Shape *ashape)
: m_loc(location), m_goal(Goal::noGoal())
{
    m_weight = weight;
    m_power = power;
    m_alive = alive;
    m_out = false;
    m_lookLeft = true;
    m_lost = false;

    m_shape = ashape;
    m_view = a_view;
    m_view->takeModel(this);
    m_rules = new Rules(this);
}
//-----------------------------------------------------------------
/**
 * Delete all member but not field.
 */
Cube::~Cube()
{
    //NOTE: rules must be destroyed before shape because they unmask self
    delete m_rules;

    delete m_view;
    delete m_shape;
}
//-----------------------------------------------------------------
/**
 * Create special model, which will be used for outher space.
 * NOTE: hack border around field
 */
    Cube *
Cube::createBorder()
{
    Cube *border = new Cube(V2(-1,-1), Cube::FIXED, Cube::NONE, false,
            new View(), new Shape("X\n"));
    border->m_view->takeModel(NULL);
    return border;
}
//-----------------------------------------------------------------
/**
 * Die.
 */
    void
Cube::change_die()
{
    LOG_INFO(ExInfo("dead")
            .addInfo("fish", toString()));
    m_alive = false;
    anim()->setEffect(ViewEffect::EFFECT_DISINTEGRATE);
}
//-----------------------------------------------------------------
/**
 * Go out of room.
 */
    void
Cube::change_goOut()
{
    //TODO: nice anim for alive object
    LOG_INFO(ExInfo("out of room")
            .addInfo("object", toString()));
    m_out = true;
    change_remove();
}
//-----------------------------------------------------------------
/**
 * Go out of room.
 */
    void
Cube::change_remove()
{
    m_lost = true;
    //NOTE: hack, object is moved out
    m_loc = V2(-1000, -1000);
    m_view->takeModel(NULL);
}
//-----------------------------------------------------------------
    void
Cube::change_turnSide()
{
    m_lookLeft = !m_lookLeft;
}

//-----------------------------------------------------------------
bool
Cube::isInvisible()
{
    return anim()->isInvisible();
}
//-----------------------------------------------------------------
void
Cube::refreshView()
{
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
    return ExInfo("model")
            .addInfo("loc", m_loc.toString())
            .addInfo("alive", m_alive)
            .addInfo("weight", m_weight)
            .addInfo("power", m_power)
            .addInfo("shape", m_shape->toString()).info();
}


