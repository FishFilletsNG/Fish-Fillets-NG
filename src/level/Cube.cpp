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
        Shape *ashape)
: m_loc(location), m_goal(Goal::noGoal())
{
    m_weight = weight;
    m_power = power;
    m_alive = alive;
    m_out = false;
    m_lookLeft = true;
    m_lost = false;

    m_shape = ashape;
    m_rules = new Rules(this);
    m_anim = new Anim();
}
//-----------------------------------------------------------------
/**
 * Delete all member but not field.
 */
Cube::~Cube()
{
    //NOTE: rules must be destroyed before shape because they unmask self
    delete m_rules;
    delete m_shape;
    delete m_anim;
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
            new Shape("X\n"));
    return border;
}
//-----------------------------------------------------------------
/**
 * Die.
 */
    void
Cube::change_die()
{
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
    m_weight = NONE;
    //NOTE: hack, object is moved out
    m_loc = V2(-1000, -1000);
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
    return m_anim->isInvisible();
}
//-----------------------------------------------------------------
    Anim *
Cube::anim()
{
    return m_anim;
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

