/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Anim.h"

#include "Log.h"
#include "Path.h"
#include "ResImagePack.h"
#include "EffectNone.h"
#include "LogicException.h"

//-----------------------------------------------------------------
/**
 * Create new animation sprite.
 */
    Anim::Anim()
: m_viewShift(0, 0)
{
    m_animPack[SIDE_LEFT] = new ResImagePack();
    m_animPack[SIDE_RIGHT] = new ResImagePack();

    m_animName = "";
    m_animPhase = 0;
    m_run = false;
    m_specialAnimName = "";
    m_specialAnimPhase = 0;
    m_effect = new EffectNone();
}
//-----------------------------------------------------------------
Anim::~Anim()
{
    m_animPack[SIDE_LEFT]->removeAll();
    m_animPack[SIDE_RIGHT]->removeAll();
    delete m_animPack[SIDE_LEFT];
    delete m_animPack[SIDE_RIGHT];
    delete m_effect;
}
//-----------------------------------------------------------------
/**
 * Draw anim phase at screen position.
 * Increase phase when anim is running.
 */
    void
Anim::drawAt(SDL_Surface *screen, int x, int y, eSide side)
{
    if (!m_effect->isInvisible()) {
        SDL_Surface *surface =
            m_animPack[side]->getRes(m_animName, m_animPhase);
        m_effect->blit(screen, surface, x, y);
        if (m_run) {
            m_animPhase++;
            if (m_animPhase >= m_animPack[side]->countRes(m_animName)) {
                m_animPhase = 0;
            }
        }

        if (!m_specialAnimName.empty()) {
            surface =
                m_animPack[side]->getRes(m_specialAnimName, m_specialAnimPhase);
            m_effect->blit(screen, surface, x, y);
            m_specialAnimName = "";
        }
    }

    m_effect->updateEffect();
}
//-----------------------------------------------------------------
/**
 * Add picture to anim,
 * default side is left side.
 */
    void
Anim::addAnim(const std::string &name, const Path &picture, eSide side)
{
    m_usedPath = picture.getPosixName();
    m_animPack[side]->addImage(name, picture);
}
//-----------------------------------------------------------------
/**
 * Run this animation.
 * Nothing is changed when animation is already running.
 */
    void
Anim::runAnim(const std::string &name, int start_phase)
{
    if (m_animName != name) {
        setAnim(name, start_phase);
    }
    m_run = true;
}
//-----------------------------------------------------------------
/**
 * Set static visage.
 */
    void
Anim::setAnim(const std::string &name, int phase)
{
    m_run = false;
    m_animName = name;
    m_animPhase = phase;

    int count = m_animPack[SIDE_LEFT]->countRes(name);
    if (m_animPhase >= count) {
        if (count == 0) {
            m_animPhase = 0;
        }
        else {
            m_animPhase %= count;
        }
        LOG_WARNING(ExInfo("anim phase over-flow")
                .addInfo("anim", name)
                .addInfo("phase", phase)
                .addInfo("count", count)
                .addInfo("usedPath", m_usedPath));
    }
}
//-----------------------------------------------------------------
/**
 * Use special efect for one phase.
 * Efect will be blited in second layer.
 *
 * @param name anim name, empty is no anim
 * @param phase anim phase
 */
    void
Anim::useSpecialAnim(const std::string &name, int phase)
{
    m_specialAnimName = name;
    m_specialAnimPhase = phase;

    int count = m_animPack[SIDE_LEFT]->countRes(name);
    if (m_specialAnimPhase >= count) {
        if (count == 0) {
            m_specialAnimName = "";
            m_specialAnimPhase = 0;
        }
        else {
            m_specialAnimPhase %= count;
        }
        LOG_WARNING(ExInfo("special anim phase over-flow")
                .addInfo("anim", name)
                .addInfo("phase", phase)
                .addInfo("count", count));
    }
}
//-----------------------------------------------------------------
/**
 * Change effect.
 * @throws LogicException when new_effect is NULL.
 */
void
Anim::changeEffect(ViewEffect *new_effect)
{
    if (NULL == new_effect) {
        throw LogicException(ExInfo("new_effect is NULL")
                .addInfo("animName", m_animName)
                .addInfo("specialAnimName", m_specialAnimName));
    }

    delete m_effect;
    m_effect = new_effect;
}
//-----------------------------------------------------------------
    int
Anim::countAnimPhases(const std::string &anim, eSide side) const
{
    return m_animPack[side]->countRes(anim);
}

