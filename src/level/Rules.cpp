/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Rules.h"

#include "Log.h"
#include "Cube.h"
#include "MarkMask.h"
#include "LayoutException.h"
#include "DialogAgent.h"

#include <assert.h>

//-----------------------------------------------------------------
/**
 * Create new rules for model.
 */
Rules::Rules(Cube *model)
{
    m_readyToDie = false;
    m_readyToTurn = false;
    m_readyToGoout = false;
    m_readyToActive = false;
    m_dir = DIR_NO;
    m_pushing = false;

    m_model = model;
    m_mask = NULL;
    m_lastFall = false;
}
//-----------------------------------------------------------------
/**
 * Unmask from field.
 */
Rules::~Rules()
{
    if (m_mask) {
        m_mask->unmask();
        delete m_mask;
    }
}
//-----------------------------------------------------------------
/**
 * Connect model with field.
 * @throws LayoutException when location is occupied
 */
    void
Rules::takeField(Field *field)
{
    if (m_mask) {
        m_mask->unmask();
        delete m_mask;
        m_mask = NULL;
    }

    m_mask = new MarkMask(m_model, field);
    if (!m_mask->getResist(DIR_NO).empty()) {
        throw LayoutException(ExInfo("position is occupied")
                .addInfo("loc", m_model->getLocation().toString()));
    }

    m_mask->mask();
}

//-----------------------------------------------------------------
/**
 * Accomplish last move in m_dir direction.
 * Mask to a new position.
 * Change model position.
 */
    void
Rules::occupyNewPos()
{
    if (m_dir != DIR_NO) {
        m_pushing = false;

        V2 shift = dir2xy(m_dir);
        V2 oldLoc = m_model->getLocation();
        m_model->change_setLocation(oldLoc.composition(shift));

        m_mask->mask();
    }
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
 * @return true when fish is dead
 */
    bool
Rules::checkDead()
{
    //TODO: after falling phase is sufficient to check only DeadFall
    bool dead = false;

    if (m_model->isAlive()) {
        if (checkDeadMove()) {
            dead = true;
        }
        else {
            if (checkDeadFall()) {
                dead = true;
            }
            else {
                dead = checkDeadStress();
            }
        }

        if (dead) {
            m_readyToDie = true;
        }
    }

    return dead;
}
//-----------------------------------------------------------------
/**
 * Check out of room objects.
 * @return true when object go out (is at border)
 */
bool
Rules::checkOut()
{
    bool result = false;
    if (!isWall() && !m_model->isLost()
            && !m_model->isBusy())
    {
        //NOTE: normal objects are not allowed to go out of screen
        if (m_model->shouldGoOut()) {
            if (m_mask->isAtBorder()) {
                m_readyToGoout = true;
                result = true;
            }
        }
    }

    return result;
}

//-----------------------------------------------------------------
/**
 * Return true when any model moves in dir != DIR_UP
 * and new position is SOLELY on a fish.
 * @return true when fish is dead
 */
    bool
Rules::checkDeadMove()
{
    Cube::t_models resist = m_mask->getResist(DIR_UP);
    Cube::t_models::iterator end = resist.end();
    for (Cube::t_models::iterator i = resist.begin(); i != end; ++i) {
        if (!(*i)->isAlive()) {
            eDir resist_dir = (*i)->rules()->getDir();
            if (resist_dir != DIR_NO && resist_dir != DIR_UP) {
                if (!(*i)->rules()->isOnStack()) {
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
 * @return true when fish is dead
 */
    bool
Rules::checkDeadFall()
{
    Cube::t_models killers = whoIsFalling();

    Cube::t_models::iterator end = killers.end();
    for (Cube::t_models::iterator i = killers.begin(); i != end; ++i) {
        if (!(*i)->rules()->isOnWall()) {
            return true;
        }
    }

    return false;
}
//-----------------------------------------------------------------
/**
 * Whether object is under hight stress.
 *
 * @return true when fish is dead
 */
    bool
Rules::checkDeadStress()
{
    Cube::t_models killers = whoIsHeavier(m_model->getPower());

    Cube::t_models::iterator end = killers.end();
    for (Cube::t_models::iterator i = killers.begin(); i != end; ++i) {
        if (!(*i)->rules()->isOnWall()) {
            if (!(*i)->rules()->isOnStrongFish((*i)->getWeight())) {
                return true;
            }
        }
    }

    return false;
}

//-----------------------------------------------------------------
/**
 * Finish events from last round.
 * Change model state.
 */
    void
Rules::changeState()
{
    m_dir = DIR_NO;

    if (m_readyToGoout) {
        m_readyToGoout = false;
        m_mask->unmask();
        m_model->change_goOut();
    }
    if (!m_model->isLost() && m_model->isDisintegrated()) {
        m_mask->unmask();
        m_model->change_remove();
    }

    if (m_readyToTurn) {
        m_readyToTurn = false;
        m_model->change_turnSide();
    }

    m_readyToActive = false;

    if (m_readyToDie) {
        m_readyToDie = false;
        m_model->change_die();
    }
}
//-----------------------------------------------------------------
/**
 * Let model fall.
 * Return whether we have fall now or in last round.
 */
    Rules::eFall
Rules::actionFall()
{
    eFall result = FALL_NO;

    if (canFall()) {
        m_dir = DIR_DOWN;
        result = FALL_NOW;
        m_lastFall = true;
    }
    else {
        if (m_lastFall) {
            result = FALL_LAST;
        }
        m_lastFall = false;
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Unmask from old position.
 */
    void
Rules::finishRound()
{
    freeOldPos();
}
//-----------------------------------------------------------------
/**
 * Unmask from old position.
 */
    void
Rules::freeOldPos()
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
Rules::canFall()
{
    bool result = false;
    if (!m_model->isLost()) {
        //NOTE: hack, apply heavy power on self
        result = canDir(DIR_DOWN, Cube::HEAVY);
    }
    return result;
}

//-----------------------------------------------------------------
bool
Rules::isWall() const
{
    return m_model->getWeight() >= Cube::FIXED;
}
//-----------------------------------------------------------------
/**
 * Whether object is on others unalive objects.
 */
    bool
Rules::isOnStack()
{
    Cube::t_models resist = m_mask->getResist(DIR_DOWN);
    Cube::t_models::iterator end = resist.end();
    for (Cube::t_models::iterator i = resist.begin(); i != end; ++i) {
        if (!(*i)->isAlive()) {
            return true;
        }
    }

    return false;
}
//-----------------------------------------------------------------
/**
 * Whether object is direct or undirect on a wall.
 */
    bool
Rules::isOnWall()
{
    bool result = false;
    if (isWall()) {
        result = true;
    }
    else if (m_model->isAlive()) {
        result = false;
    }
    else {
        m_mask->unmask();

        result = false;
        Cube::t_models resist = m_mask->getResist(DIR_DOWN);
        Cube::t_models::iterator end = resist.end();
        for (Cube::t_models::iterator i = resist.begin(); i != end; ++i) {
            if ((*i)->rules()->isOnWall()) {
                //NOTE: don't forget to mask()
                result = true;
                break;
            }
        }

        m_mask->mask();
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Whether object is direct or undirect on a strong fish.
 *
 * @param weight stress weight which must fish carry
 * @return whether a strong fish carry this object
 */
    bool
Rules::isOnStrongFish(Cube::eWeight weight)
{
    bool result = false;
    if (m_model->isAlive()) {
        result = (m_model->getPower() >= weight);
    }
    else {
        m_mask->unmask();

        result = false;
        Cube::t_models resist = m_mask->getResist(DIR_DOWN);
        Cube::t_models::iterator end = resist.end();
        for (Cube::t_models::iterator i = resist.begin(); i != end; ++i) {
            if ((*i)->rules()->isOnStrongFish(weight)) {
                //NOTE: don't forget to mask()
                result = true;
                break;
            }
        }

        m_mask->mask();
    }

    return result;
}

//-----------------------------------------------------------------
/**
 * Whether object is falling.
 */
bool
Rules::isFalling() const
{
    bool result = false;
    if (!m_model->isAlive()) {
        result = (m_dir == DIR_DOWN);
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Who is falling on us.
 * @return array of killers, they can fall undirect on us
 */
    Cube::t_models 
Rules::whoIsFalling() 
{
    Cube::t_models result;
    m_mask->unmask();

    Cube::t_models resist = m_mask->getResist(DIR_UP);
    Cube::t_models::iterator end = resist.end();
    for (Cube::t_models::iterator i = resist.begin(); i != end; ++i) {
        //NOTE: falling is propagated over fish
        if (!(*i)->rules()->isWall()) {
            if ((*i)->rules()->isFalling()) {
                result.push_back(*i);
            }
            else {
                Cube::t_models distance_killers = (*i)->rules()->whoIsFalling();
                result.insert(result.end(), distance_killers.begin(),
                        distance_killers.end());
            }
        }
    }

    m_mask->mask();
    return result;
}
//-----------------------------------------------------------------
/**
 * Whether object is heavier than our power.
 * @param power our max power
 * @return whether object is heavier
 */
bool
Rules::isHeavier(Cube::eWeight power) const
{
    bool result = false;
    if (!isWall() && !m_model->isAlive()) {
        if (m_model->getWeight() > power) {
            result = true;
        }
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Who is heavier than our power.
 * @param power our max power
 * @return array of killers, they can lie undirect on us
 */
    Cube::t_models
Rules::whoIsHeavier(Cube::eWeight power)
{
    Cube::t_models result;
    m_mask->unmask();

    Cube::t_models resist = m_mask->getResist(DIR_UP);
    Cube::t_models::iterator end = resist.end();
    for (Cube::t_models::iterator i = resist.begin(); i != end; ++i) {
        if (!(*i)->rules()->isWall()) {
            if ((*i)->rules()->isHeavier(power)) {
                result.push_back(*i);
            }
            else {
                Cube::t_models distance_killers =
                    (*i)->rules()->whoIsHeavier(power);
                result.insert(result.end(), distance_killers.begin(),
                        distance_killers.end());
            }
        }
    }

    m_mask->mask();
    return result;
}

//-----------------------------------------------------------------
/**
 * Whether other will retreat before us.
 *
 * @param power we will use this power
 */
    bool
Rules::canMoveOthers(eDir dir, Cube::eWeight power)
{
    bool result = true;
    //NOTE: make place after oneself, e.g. fish in U
    m_mask->unmask();

    Cube::t_models resist = m_mask->getResist(dir);
    Cube::t_models::iterator end = resist.end();
    for (Cube::t_models::iterator i = resist.begin(); i != end; ++i) {
        if (!(*i)->rules()->canDir(dir, power)) {
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
Rules::canDir(eDir dir, Cube::eWeight power)
{
    bool result = false;
    if (!m_model->isAlive() && power >= m_model->getWeight()) {
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
Rules::actionMoveDir(eDir dir)
{
    bool result = false;
    if (canMoveOthers(dir, m_model->getPower())) {
        moveDirBrute(dir);
        result = true;
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Irrespective move.
 * Set m_dir to this dir and do the same for all resist.
 * Only m_dir and m_pushing will be set.
 */
    void
Rules::moveDirBrute(eDir dir)
{
    //NOTE: make place after oneself, e.g. object in U
    m_mask->unmask();

    Cube::t_models resist = m_mask->getResist(dir);
    Cube::t_models::iterator end = resist.end();
    if (end != resist.begin()) {
        m_pushing = true;
    }
    for (Cube::t_models::iterator i = resist.begin(); i != end; ++i) {
        (*i)->rules()->moveDirBrute(dir);
    }

    m_dir = dir;
    m_mask->mask();
}

//-----------------------------------------------------------------
/**
 * Convert dir to relative coordinations.
 */
V2
Rules::dir2xy(eDir dir)
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
            assert(!"unknown dir");
            break;
    }

    return V2(x, y);
}
//-----------------------------------------------------------------
/**
 * Return what we do the last round.
 * NOTE: dead is not action
 */
std::string
Rules::getAction() const
{
    if (m_readyToGoout) {
        return "goout";
    }
    else if (m_readyToTurn) {
        return "turn";
    }
    else if (m_readyToActive) {
        return "activate";
    }
    else if (m_model->isBusy()) {
        return "busy";
    }

    switch (m_dir) {
        case DIR_LEFT: return "move_left";
        case DIR_RIGHT: return "move_right";
        case DIR_UP: return "move_up";
        case DIR_DOWN: return "move_down";
        case DIR_NO: return "rest";
        default: assert(!"unknown dir"); break;
    }

    return "rest";
}
//-----------------------------------------------------------------
/**
 * Return how we have feel the last round.
 */
std::string
Rules::getState() const
{
    if (!m_model->isAlive()) {
        return "dead";
    }
    else if (m_pushing) {
        return "pushing";
    }
    else if (DialogAgent::agent()->isTalking(m_model->getIndex())) {
        return "talking";
    }
    else {
        return "normal";
    }
}


