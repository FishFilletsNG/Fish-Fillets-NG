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
#include "ResImagePack.h"

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
}
//-----------------------------------------------------------------
Anim::~Anim()
{
    m_animPack[SIDE_LEFT]->removeAll();
    m_animPack[SIDE_RIGHT]->removeAll();
    delete m_animPack[SIDE_LEFT];
    delete m_animPack[SIDE_RIGHT];
}
//-----------------------------------------------------------------
/**
 * Draw anim phase at screen position.
 * Increase phase when anim is running.
 */
    void
Anim::drawAt(SDL_Surface *screen, int x, int y, eSide side)
{
    SDL_Surface *surface =
        m_animPack[side]->getRes(m_animName, m_animPhase);
    m_effect.blit(screen, surface, x, y);
    if (m_run) {
        m_animPhase++;
        if (m_animPhase >= m_animPack[side]->countRes(m_animName)) {
            m_animPhase = 0;
        }
    }

    if (!m_specialAnimName.empty()) {
        surface =
            m_animPack[side]->getRes(m_specialAnimName, m_specialAnimPhase);
        m_effect.blit(screen, surface, x, y);
        m_specialAnimName = "";
    }

    m_effect.updateEffect();
}
//-----------------------------------------------------------------
/**
 * Add picture to anim,
 * default side is left side.
 */
    void
Anim::addAnim(const std::string &name, const Path &picture)
{
    m_animPack[SIDE_LEFT]->addImage(name, picture);
}
//-----------------------------------------------------------------
    void
Anim::addDuplexAnim(const std::string &name,
        const Path &left_picture, const Path &right_picture)
{
    m_animPack[SIDE_LEFT]->addImage(name, left_picture);
    m_animPack[SIDE_RIGHT]->addImage(name, right_picture);
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
        m_animPhase %= count;
        LOG_WARNING(ExInfo("anim phase over-flow")
                .addInfo("anim", name)
                .addInfo("phase", phase)
                .addInfo("count", count));
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
        m_specialAnimPhase %= count;
        LOG_WARNING(ExInfo("special anim phase over-flow")
                .addInfo("anim", name)
                .addInfo("phase", phase)
                .addInfo("count", count));
    }
}

