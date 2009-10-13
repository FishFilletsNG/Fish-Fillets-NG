/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Cube.h"

#include "Shape.h"
#include "Rules.h"
#include "LayoutException.h"
#include "Anim.h"
#include "EffectDisintegrate.h"
#include "DialogStack.h"

//-----------------------------------------------------------------
/**
 * Create new model.
 */
Cube::Cube(const V2 &location,
        eWeight weight, eWeight power, bool alive,
        Shape *new_shape)
: m_loc(location), m_goal(Goal::noGoal())
{
    m_index = -1;
    m_busy = false;

    m_weight = weight;
    m_power = power;
    m_alive = alive;
    m_out = false;
    m_lookLeft = true;
    m_lost = false;
    m_outDir = Dir::DIR_NO;
    m_outCapacity = 0;

    m_shape = new_shape;
    m_rules = new Rules(this);
    m_anim = new Anim();
    m_dialogs = NULL;
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
 * Die.
 */
    void
Cube::change_die()
{
    m_alive = false;
    anim()->changeEffect(new EffectDisintegrate());
}
//-----------------------------------------------------------------
/**
 * Make the object unmovable
 * when it is going out of the room.
 */
    void
Cube::change_goingOut()
{
    m_weight = Cube::FIXED;
}
//-----------------------------------------------------------------
/**
 * Go out of room.
 */
    void
Cube::change_goOut()
{
    m_out = true;
    change_remove();
}
//-----------------------------------------------------------------
/**
 * Go out of game, e.g. disintegrated skeleton.
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
Dir::eDir
Cube::getLastMoveDir() const
{
    return m_rules->getDir();
}
//-----------------------------------------------------------------
/**
 * Set params for special 'output_DIR' model.
 * Used just for the 'spuntik' in windoze level.
 */
void
Cube::setOutDir(Dir::eDir dir, int capacity, eWeight weight)
{
    m_outCapacity = capacity;
    m_outDir = dir;
    m_weight = weight;
}
//-----------------------------------------------------------------
/**
 * Set extra parameters from a saved undo state.
 * They have to restore just the parameters used by the View.
 */
void
Cube::setExtraParams()
{
    m_lost = false;
    m_rules->resetLastDir();
}
//-----------------------------------------------------------------
/**
 * Special model 'output_DIR' has capacity to absorb two fishes,
 * then it changes to normal 'item_light'.
 */
void
Cube::decOutCapacity()
{
    if (m_outCapacity > 0) {
        m_outCapacity--;
        if (m_outCapacity == 0) {
            m_outDir = Dir::DIR_NO;
            m_weight = LIGHT;
            m_outCapacity = -1;
        }
    }
}
//-----------------------------------------------------------------
bool
Cube::isDisintegrated()
{
    return m_anim->isDisintegrated();
}
//-----------------------------------------------------------------
bool
Cube::isInvisible()
{
    return m_anim->isInvisible();
}
//-----------------------------------------------------------------
bool
Cube::isTalking() const
{
    return (m_dialogs && m_dialogs->isTalking(m_index));
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


