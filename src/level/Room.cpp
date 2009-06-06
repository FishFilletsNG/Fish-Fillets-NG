/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Room.h"

#include "WavyPicture.h"
#include "Field.h"
#include "FinderAlg.h"
#include "ResSoundPack.h"
#include "Controls.h"
#include "PhaseLocker.h"
#include "Planner.h"
#include "View.h"

#include "Log.h"
#include "Rules.h"
#include "LogicException.h"
#include "LoadException.h"
#include "Unit.h"
#include "TimerAgent.h"
#include "SubTitleAgent.h"
#include "DialogStack.h"
#include "SoundAgent.h"
#include "OptionAgent.h"
#include "ModelList.h"
#include "Landslip.h"
#include "MouseStroke.h"
#include "MouseControl.h"
#include "Path.h"

#include <assert.h>

//-----------------------------------------------------------------
/**
 * Create room holder.
 * @param w room width
 * @param h room height
 * @param picture room background
 * @param locker shared locker for anim
 * @param levelScript shared planner to interrupt
 */
Room::Room(int w, int h, const std::string &picture,
        PhaseLocker *locker, Planner *levelScript)
{
    m_locker = locker;
    m_levelScript = levelScript;
    m_fastFalling = false;
    m_bg = new WavyPicture(Path::dataReadPath(picture), V2(0, 0));
    m_bgFilename = picture;
    m_field = new Field(w, h);
    m_finder = new FinderAlg(w, h);
    m_controls = new Controls(m_locker);
    m_view = new View(ModelList(&m_models));
    m_lastAction = Cube::ACTION_NO;
    m_soundPack = new ResSoundPack();
    m_startTime = TimerAgent::agent()->getCycles();
}
//-----------------------------------------------------------------
/**
 * Delete field and models.
 */
Room::~Room()
{
    m_soundPack->removeAll();
    delete m_soundPack;
    m_levelScript->killPlan();
    m_levelScript->dialogs()->removeAll();
    SubTitleAgent::agent()->removeAll();
    delete m_controls;
    delete m_view;

    //NOTE: models must be removed before field because they unmask self
    Cube::t_models::iterator end = m_models.end();
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        delete (*i);
    }

    delete m_finder;
    delete m_field;
    delete m_bg;
}
//-----------------------------------------------------------------
/**
 * Set waves on background.
 */
    void
Room::setWaves(float amplitude, float periode, float speed)
{
    m_bg->setWamp(amplitude);
    m_bg->setWperiode(periode);
    m_bg->setWspeed(speed);
}
//-----------------------------------------------------------------
    void
Room::addDecor(Decor *new_decor)
{
    m_view->addDecor(new_decor);
}
//-----------------------------------------------------------------
/**
 * Add model at scene.
 * @param new_model new object
 * @param new_unit driver for the object or NULL
 * @return model index
 */
    int
Room::addModel(Cube *new_model, Unit *new_unit)
{
    new_model->rules()->takeField(m_field);
    m_models.push_back(new_model);

    if (new_unit) {
        new_unit->takeModel(new_model);
        m_controls->addUnit(new_unit);
    }

    int model_index = m_models.size() - 1;
    new_model->setIndex(model_index);
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
 */
    void
Room::nextRound(const InputProvider *input)
{
    if (m_fastFalling) {
        while (beginFall()) {
            finishRound();
        }
    }
    else {
        beginFall();
    }

    if (isFresh()) {
        if (m_controls->driving(input)) {
            m_lastAction = Cube::ACTION_MOVE;
        }
        else {
            MouseControl rat(m_controls, m_view, m_finder);
            if (rat.mouseDrive(input)) {
                m_lastAction = Cube::ACTION_MOVE;
            }
        }
    }
    finishRound();
}
//-----------------------------------------------------------------
/**
 * Play sound like some object has fall.
 * NOTE: only one sound is played even more objects have fall
 */
    void
Room::playImpact(Cube::eWeight impact)
{
    switch (impact) {
        case Cube::NONE:
            break;
        case Cube::LIGHT:
            playSound("impact_light", 50);
            break;
        case Cube::HEAVY:
            playSound("impact_heavy", 50);
            break;
        default:
            assert(!"unknown impact weight");
    }
}
//-----------------------------------------------------------------
/**
 * Play sound like a fish die.
 * @param model fresh dead fish
 */
    void
Room::playDead(Cube *model)
{
    m_levelScript->dialogs()->killSound(model->getIndex());
    switch (model->getPower()) {
        case Cube::LIGHT:
            playSound("dead_small");
            break;
        case Cube::HEAVY:
            playSound("dead_big");
            break;
        default:
            LOG_WARNING(ExInfo("curious power of dead fish")
                    .addInfo("power", model->getPower()));
            break;
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
    bool interrupt = false;

    //NOTE: we must call this functions sequential for all objects
    Cube::t_models::iterator end = m_models.end();
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        (*i)->rules()->occupyNewPos();
    }
    for (Cube::t_models::iterator j = m_models.begin(); j != end; ++j) {
        bool die = (*j)->rules()->checkDead(m_lastAction);
        interrupt |= die;
        if (die) {
            playDead(*j);
        }
    }
    for (Cube::t_models::iterator l = m_models.begin(); l != end; ++l) {
        (*l)->rules()->changeState();
    }

    if (interrupt) {
        m_levelScript->interruptPlan();
    }
}
//-----------------------------------------------------------------
/**
 * Let models to go out of screen.
 * @param interactive whether do anim
 * @return true when a model went out
 */
    bool
Room::fallout(bool interactive)
{
    bool wentOut = false;
    Cube::t_models::iterator end = m_models.end();
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        if (!(*i)->isLost()) {
            int outDepth = (*i)->rules()->actionOut();
            if (outDepth > 0) {
                wentOut = true;
                if (interactive) {
                    m_locker->ensurePhases(3);
                }
            }
            else if (outDepth == -1) {
                m_levelScript->interruptPlan();
            }
        }
    }

    return wentOut;
}
//-----------------------------------------------------------------
/**
 * Let things fall.
 * @return true when something is falling.
 */
    bool
Room::falldown(bool interactive)
{
    ModelList models(&m_models);
    Landslip slip(models);

    bool falling = slip.computeFall();
    if (interactive) {
        playImpact(slip.getImpact());
    }
    return falling;
}
//-----------------------------------------------------------------
/**
 * Let models to release their old position.
 * @param interactive whether ensure phases for motion animation
 */
    void
Room::finishRound(bool interactive)
{
    if (interactive) {
        m_controls->lockPhases();
    }
    m_view->noteNewRound(m_locker->getLocked());

    Cube::t_models::iterator end = m_models.end();
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        (*i)->rules()->finishRound();
    }
}

//-----------------------------------------------------------------
    void
Room::switchFish()
{
    m_controls->switchActive();
}
//-----------------------------------------------------------------
    void
Room::controlEvent(const KeyStroke &stroke)
{
    m_controls->controlEvent(stroke);
}
//-----------------------------------------------------------------
    void
Room::controlMouse(const MouseStroke &button)
{
    if (button.isLeft()) {
        V2 fieldPos = m_view->getFieldPos(button.getLoc());
        Cube *model = askField(fieldPos);
        m_controls->activateSelected(model);
    }
}

//-----------------------------------------------------------------
const StepCounter *
Room::stepCounter() const
{
    return m_controls;
}
//-----------------------------------------------------------------
    void
Room::setMoves(const std::string &moves)
{
    m_controls->setMoves(moves);
}
//-----------------------------------------------------------------
    void
Room::checkActive()
{
    return m_controls->checkActive();
}
//-----------------------------------------------------------------
    void
Room::unBusyUnits()
{
    Cube::t_models::iterator end = m_models.end();
    for (Cube::t_models::iterator i = m_models.begin(); i != end; ++i) {
        (*i)->setBusy(false);
    }
}
//-----------------------------------------------------------------
/**
 * Load this move, let object to fall fast.
 * Don't play sound.
 * @throws LoadException for bad moves
 */
    void
Room::loadMove(char move)
{
    static const bool NO_INTERACTIVE = false;
    bool falling = true;
    while (falling) {
        falling = beginFall(NO_INTERACTIVE);
        makeMove(move);

        finishRound(NO_INTERACTIVE);
    }
}
//-----------------------------------------------------------------
/**
 * Begin round.
 * Let objects fall.
 * First objects can fall out of room (even upward),
 * when nothing is going out, then objects can fall down by gravity.
 *
 * @param interactive whether play sound and do anim
 * @return true when something was falling
 */
    bool
Room::beginFall(bool interactive)
{
    prepareRound();
    m_lastAction = Cube::ACTION_NO;

    if (fallout(interactive)) {
        m_lastAction = Cube::ACTION_MOVE;
    }
    else {
        if (falldown(interactive)) {
            m_lastAction = Cube::ACTION_FALL;
        }
    }
    return m_lastAction != Cube::ACTION_NO;
}
//-----------------------------------------------------------------
/**
 * Try make single move.
 * @return true for success or false when something has moved before
 * @throws LoadException for bad moves
 */
    bool
Room::makeMove(char move)
{
    bool result = false;
    if (isFresh()) {
        if (!m_controls->makeMove(move)) {
            throw LoadException(ExInfo("load error - bad move")
                    .addInfo("move", std::string(1, move)));
        }
        m_lastAction = Cube::ACTION_MOVE;
        result = true;
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Returns true when there is no unit which will be able to move.
 */
bool
Room::cannotMove() const
{
    return m_controls->cannotMove();
}
//-----------------------------------------------------------------
/**
 * Returns true when all goals can be solved.
 */
bool
Room::isSolvable() const
{
    Cube::t_models::const_iterator end = m_models.end();
    for (Cube::t_models::const_iterator i = m_models.begin(); i != end; ++i) {
        if ((*i)->isWrong()) {
            return false;
        }
    }
    return true;
}
//-----------------------------------------------------------------
/**
 * Returns true when all goal all satisfied.
 * Right time to ask is after finishRound.
 * NOTE: room is not solved when somethig is still falling
 */
bool
Room::isSolved() const
{
    if (!isFresh()) {
        return false;
    }
    Cube::t_models::const_iterator end = m_models.end();
    for (Cube::t_models::const_iterator i = m_models.begin(); i != end; ++i) {
        if (!(*i)->isSatisfy()) {
            return false;
        }
    }
    return true;
}

//-----------------------------------------------------------------
int
Room::getW() const
{
    return m_field->getW();
}
//-----------------------------------------------------------------
int
Room::getH() const
{
    return m_field->getH();
}
//-----------------------------------------------------------------
int
Room::getCycles() const
{
    return TimerAgent::agent()->getCycles() - m_startTime;
}
//-----------------------------------------------------------------
    void
Room::addSound(const std::string &name, const Path &file)
{
    m_soundPack->addSound(name, file);
}
//-----------------------------------------------------------------
    void
Room::playSound(const std::string &name, int volume)
{
    if (OptionAgent::agent()->getAsBool("sound", true)) {
        SoundAgent::agent()->playSound(
            m_soundPack->getRandomRes(name), volume);
    }
}
//-----------------------------------------------------------------
/**
 * Shift room content.
 * NOTE: background is not shifted
 */
    void
Room::setScreenShift(const V2 &shift)
{
    m_view->setScreenShift(shift);
}
//-----------------------------------------------------------------
void
Room::changeBg(const std::string &picture)
{
    if (picture != m_bgFilename) {
        m_bg->changePicture(Path::dataReadPath(picture));
        m_bgFilename = picture;
    }
}
//-----------------------------------------------------------------
    void
Room::drawOn(SDL_Surface *screen)
{
    m_bg->drawOn(screen);
    m_view->drawOn(screen);
}

