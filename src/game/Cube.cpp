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
#include "MarkMask.h"
#include "Shape.h"
#include "Driver.h"
#include "ResSoundAgent.h"
#include "SoundAgent.h"
#include "LayoutException.h"

#include <assert.h>

//-----------------------------------------------------------------
/**
 * Create new model.
 * Set the mask later with takeField().
 */
Cube::Cube(const V2 &location,
        eWeight weight, eWeight power, bool alive,
        Driver *driver, View *a_view, Shape *shape)
: m_loc(location)
{
    m_weight = weight;
    m_power = power;
    m_alive = alive;
    m_readyToDie = false;
    m_dir = DIR_NO;

    m_shape = shape;
    m_driver = driver;
    m_view = a_view;
    m_view->takeModel(this);
    m_mask = NULL;

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
    if (m_mask) {
        m_mask->unmask();
        delete m_mask;
    }

    delete m_shape;
    delete m_driver;
    delete m_view;
}
//-----------------------------------------------------------------
/**
 * Connect model with field.
 * @throws LayoutException when location is occupied
 */
    void
Cube::takeField(Field *field)
{
    if (m_mask) {
        m_mask->unmask();
        delete m_mask;
        m_mask = NULL;
    }

    m_mask = new MarkMask(this, field);
    if (false == m_mask->getResist(DIR_NO).empty()) {
        throw LayoutException(ExInfo("position is occupied")
                .addInfo("x", m_loc.getX())
                .addInfo("y", m_loc.getY()));
    }

    m_mask->mask();
}
//-----------------------------------------------------------------
/**
 * Accomplish last move in m_dir direction.
 * Mask to a new position.
 */
    void
Cube::occupyNewPos()
{
    if (m_dir != DIR_NO) {
        int x;
        int y;
        dir2xy(m_dir, &x, &y);
        m_loc = V2(m_loc.getX() + x, m_loc.getY() + y);

        m_mask->mask();
    }

    m_view->prepareRound();
}
//-----------------------------------------------------------------
/**
 * Check dead fishes.
 * Fish is dead:
 * - when any model moves in dir != DIR_UP
 *   and new position is SOLELY on a fish
 * - when any model moves in DIR_DOWN
 *   and new position is SOLELY on models SOLELY on a fish
 * - when any model rests SOLELY on models SOLELY on a fish
 *   with fish.power < model.weight
 * 
 */
    void
Cube::checkDead()
{
    //TODO: after falling phase is sufficient to check only DeadFall
    //FIXME: can one dead fish kill another fish in the same round?
    if (m_alive) {
        bool dead = false;
        if (checkDeadMove()) {
            dead = true;
        }
        else {
            if (checkDeadFall()) {
                dead = true;
            }
            else {
                dead = checkDeadStress(m_power);
            }
        }

        if (dead) {
            m_readyToDie = true;
        }
    }
}
//-----------------------------------------------------------------
/**
 * Return true when dead.
 */
    bool
Cube::checkDeadMove()
{
    Cube::t_models resist = m_mask->getResist(DIR_UP);
    Cube::t_models::iterator end = resist.end();
    for (t_models::iterator i = resist.begin(); i != end; ++i) {
        if (false == (*i)->isAlive()) {
            eDir resist_dir = (*i)->getDir();
            if (resist_dir != DIR_NO && resist_dir != DIR_UP) {
                if (false == (*i)->isOnStack()) {
                    return true;
                }
            }
        }
    }

    return false;
}
//-----------------------------------------------------------------
/**
 * Whether object is under falling object.
 *
 * @return true when dead
 */
    bool
Cube::checkDeadFall()
{
    Cube::t_models resist = m_mask->getResist(DIR_UP);
    Cube::t_models::iterator end = resist.end();
    for (t_models::iterator i = resist.begin(); i != end; ++i) {
        if ((*i)->isFallingOnFish()) {
            return true;
        }
    }

    return false;
}
//-----------------------------------------------------------------
/**
 * Whether object is under hight stress.
 *
 * @param power our max power
 * @return true when dead
 */
    bool
Cube::checkDeadStress(eWeight power)
{
    Cube::t_models resist = m_mask->getResist(DIR_UP);
    Cube::t_models::iterator end = resist.end();
    for (t_models::iterator i = resist.begin(); i != end; ++i) {
        if ((*i)->isHeavier(power)) {
            return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------
/**
 * Make skeletons.
 */
    void
Cube::applyDead()
{
    m_dir = DIR_NO;

    if (m_readyToDie) {
        //TODO: nice dead
        LOG_INFO(ExInfo("dead")
                .addInfo("fish", toString()));
        m_readyToDie = false;
        m_alive = false;
        m_view->setAnim("skeleton");
        SoundAgent::agent()->playSound("xplo");
    }
}
//-----------------------------------------------------------------
/**
 * Let model fall.
 * Return true when we have fall.
 */
    bool
Cube::fall()
{
    bool result = false;
    if (canFall()) {
        m_dir = DIR_DOWN;
        result = true;
    }
    return result;
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
 */
    void
Cube::freeOldPos()
{
    if (m_dir != DIR_NO) {
        m_mask->unmask();
    }
}

//-----------------------------------------------------------------
/**
 * Whether object can fall.
 * Not alive model and not FIXED object falls down.
 */
    bool
Cube::canFall()
{
    //NOTE: hack with heavy power
    return canDir(DIR_DOWN, HEAVY);
}
//-----------------------------------------------------------------
/**
 * Whether object is on others unalive objects.
 */
    bool
Cube::isOnStack()
{
    bool result = false;
    Cube::t_models resist = m_mask->getResist(DIR_DOWN);
    Cube::t_models::iterator end = resist.end();
    for (t_models::iterator i = resist.begin(); i != end; ++i) {
        if (false == (*i)->isAlive()) {
            result = true;
            break;
        }
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Whether object is direct or undirect on a wall.
 */
    bool
Cube::isOnWall()
{
    bool result = false;
    if (isWall()) {
        result = true;
    }
    else {
        result = false;
        Cube::t_models resist = m_mask->getResist(DIR_DOWN);
        Cube::t_models::iterator end = resist.end();
        for (t_models::iterator i = resist.begin(); i != end; ++i) {
            if ((*i)->isOnWall()) {
                result = true;
                break;
            }
        }
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Whether object is direct or undirect on a wall or strong fish.
 *
 * @param weight stress weight which must wall or fish carry
 */
    bool
Cube::isOnStrong(eWeight weight)
{
    bool result = false;
    if (m_alive) {
        result = (m_power >= weight);
    }
    else {
        if (isWall()) {
            result = true;
        }
        else {
            result = false;
            Cube::t_models resist = m_mask->getResist(DIR_DOWN);
            Cube::t_models::iterator end = resist.end();
            for (t_models::iterator i = resist.begin(); i != end; ++i) {
                if ((*i)->isOnStrong(weight)) {
                    result = true;
                    break;
                }
            }
        }
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Whether object is falling
 * or something is falling on it
 * and is SOLELY on objects SOLELY on a fish.
 */
    bool
Cube::isFallingOnFish()
{
    bool result = false;
    if (false == isWall() && false == m_alive) {
        if (m_dir == DIR_DOWN) {
            result = (false == isOnWall());
        }
        else {
            //TODO: can we check only objects with DIR_NO?
            result = checkDeadFall();
        }
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Whether object is heavier than our power
 * or there is heavier object above it
 * and there is no holder under it except a weak fish.
 *
 * @param power our max power
 */
    bool
Cube::isHeavier(eWeight power)
{
    bool result = false;
    if (false == isWall() && false == m_alive) {
        //NOTE: there can be heavier model which even strong fish cannot carry
        if (m_weight > power && false == isOnStrong(m_weight)) {
            result = true;
        }
        else {
            result = checkDeadStress(power);
        }
    }

    return result;
}

//-----------------------------------------------------------------
/**
 * Whether other will retreat before us.
 *
 * @param power we will use this power
 */
    bool
Cube::canMoveOthers(eDir dir, eWeight power)
{
    bool result = true;
    //NOTE: make place after oneself, e.g. fish in U
    m_mask->unmask();

    Cube::t_models resist = m_mask->getResist(dir);
    Cube::t_models::iterator end = resist.end();
    for (t_models::iterator i = resist.begin(); i != end; ++i) {
        if (false == (*i)->canDir(dir, power)) {
            result = false;
            break;
        }
    }

    m_mask->mask();
    return result;
}
//-----------------------------------------------------------------
/**
 * Whether others can move us.
 * Live models cannot be moved by others.
 * Power and weight is compared.
 *
 * @param dir move direction
 * @param power others power
 * @return whether we can move
 */
    bool
Cube::canDir(eDir dir, eWeight power)
{
    bool result = false;
    if (false == m_alive && power >= m_weight) {
        result = canMoveOthers(dir, power);
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Try to move.
 * Only m_dir will be set.
 * NOTE: we can move all resist or none
 * 
 * @return whether we have moved
 */
    bool
Cube::moveDir(eDir dir)
{
    bool result = false;
    if (canMoveOthers(dir, m_power)) {
        moveDirBrute(dir);
        result = true;
    }
    else {
        //TODO: proper sounds
        if (m_alive) {
            SoundAgent::agent()->playSound("xplo");
        }
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Irrespective move.
 * Set m_dir to this dir and do the same for all resist.
 * Only m_dir will be set.
 */
    void
Cube::moveDirBrute(eDir dir)
{
    Cube::t_models resist = m_mask->getResist(dir);
    Cube::t_models::iterator end = resist.end();
    for (t_models::iterator i = resist.begin(); i != end; ++i) {
        (*i)->moveDirBrute(dir);
    }

    m_dir = dir;
}

//-----------------------------------------------------------------
/**
 * Convert dir to relative coordinations.
 */
void
Cube::dir2xy(eDir dir, int *out_x, int *out_y) const
{
    int x = 0;
    int y = 0;
    switch (dir) {
        case DIR_UP:
            y = -1;
            break;
        case DIR_DOWN:
            y = +1;
            break;
        case DIR_LEFT:
            x = -1;
            break;
        case DIR_RIGHT:
            x = +1;
            break;
        case DIR_NO:
            break;
        default:
            assert(NULL == "unkown dir");
            break;
    }

    *out_x = x;
    *out_y = y;
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


