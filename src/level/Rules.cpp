/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Rules.h"

#include "Cube.h"
#include "MarkMask.h"

#include "Log.h"
#include "LayoutException.h"
#include "DialogAgent.h"
#include "OnStack.h"
#include "OnWall.h"
#include "OnStrongPad.h"

#include <assert.h>

//-----------------------------------------------------------------
/**
 * Create new rules for model.
 */
Rules::Rules(Cube *model)
{
    m_readyToDie = false;
    m_readyToTurn = false;
    m_readyToActive = false;
    m_dir = DIR_NO;
    m_pushing = false;
    m_outDepth = 0;

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
    Cube::t_models resist = m_mask->getResist(DIR_NO);
    if (!resist.empty()) {
        throw LayoutException(ExInfo("position is occupied")
                .addInfo("model", m_model->toString())
                .addInfo("resist", resist.front()->toString()));
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
        m_model->change_setLocation(oldLoc.plus(shift));

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
        if (!(*i)->rules()->isOnStrongPad((*i)->getWeight())) {
            return true;
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
 * Let model to go out of room.
 * @return out depth, 0 for normal, 1 for going out,
 * 2... for on the way, -1 for out of screen
 */
int
Rules::actionOut()
{
    if (!m_model->isWall() && !m_model->isLost()
            && !m_model->isBusy())
    {
        //NOTE: normal objects are not allowed to go out of screen
        if (m_model->shouldGoOut()) {
            eDir borderDir = m_mask->getBorderDir();
            if (borderDir != DIR_NO) {
                if (isFreeDir(borderDir)) {
                    m_dir = borderDir;
                    m_outDepth += 1;
                }
            }
            else {
                if (m_outDepth > 0) {
                    m_model->change_goOut();
                    m_outDepth = -1;
                }
            }
        }
    }

    return m_outDepth;
}
//-----------------------------------------------------------------
/**
 * Let model fall.
 */
    void
Rules::actionFall()
{
    m_dir = DIR_DOWN;
    m_lastFall = true;
}
//-----------------------------------------------------------------
/**
 * Unset falling flag.
 * @return last value of the flag
 */
    bool
Rules::clearLastFall()
{
    bool last = m_lastFall;
    m_lastFall = false;
    return last;
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
 * Whether there is no resist.
 */
    bool
Rules::isFreeDir(eDir dir)
{
    m_mask->unmask();
    Cube::t_models resist = m_mask->getResist(dir);
    m_mask->mask();
    return resist.empty();
}
//-----------------------------------------------------------------
/**
 * Whether object is direct or undirect on something specific.
 * @param cond condition which will be satify when object is on.
 */
    bool
Rules::isOnCond(const OnCondition &cond)
{
    bool result = false;
    if (cond.isSatisfy(m_model)) {
        result = true;
    }
    else if (cond.isWrong(m_model)) {
        result = false;
    }
    else {
        m_mask->unmask();

        result = false;
        Cube::t_models resist = m_mask->getResist(DIR_DOWN);
        Cube::t_models::iterator end = resist.end();
        for (Cube::t_models::iterator i = resist.begin(); i != end; ++i) {
            if ((*i)->rules()->isOnCond(cond)) {
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
 * Whether object is on another unalive object.
 * And this another object must be on something fixed
 * (except on original object).
 *
 * Such object can be moved over fish.
 */
    bool
Rules::isOnStack()
{
    return isOnCond(OnStack());
}
//-----------------------------------------------------------------
/**
 * Whether object is direct or undirect on a wall.
 */
    bool
Rules::isOnWall()
{
    return isOnCond(OnWall());
}
//-----------------------------------------------------------------
/**
 * Whether object is direct or undirect on Wall or on powerful fish.
 *
 * @param weight stress weight which must fish carry
 * @return whether Wall or a strong fish carry this object
 */
    bool
Rules::isOnStrongPad(Cube::eWeight weight)
{
    return isOnCond(OnStrongPad(weight));
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
        if (!(*i)->isWall()) {
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
    if (!m_model->isWall() && !m_model->isAlive()) {
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
        if (!(*i)->isWall()) {
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
    if (m_readyToTurn) {
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
    else if (DialogAgent::agent()->isTalking(m_model->getIndex())) {
        return "talking";
    }
    else if (m_outDepth == 1) {
        return "goout";
    }
    else if (m_pushing) {
        return "pushing";
    }
    else {
        return "normal";
    }
}
//-----------------------------------------------------------------
bool
Rules::isAtBorder() const
{
    return m_mask->getBorderDir() != DIR_NO;
}
//-----------------------------------------------------------------
const Cube::t_models
Rules::getResist(eDir dir) const
{
    return m_mask->getResist(dir);
}

