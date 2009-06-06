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
#include "LogicException.h"
#include "StringTool.h"

#include "EffectNone.h"
#include "EffectMirror.h"
#include "EffectInvisible.h"
#include "EffectReverse.h"
#include "EffectZx.h"


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
 * Add prepared picture to anim,
 * default side is left side.
 */
    void
Anim::addAnim(const std::string &name, SDL_Surface *new_image, eSide side)
{
    m_animPack[side]->addRes(name, new_image);
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
    if (m_specialAnimName.empty()) {
        return;
    }

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
//-----------------------------------------------------------------
    void
Anim::setEffect(const std::string &effectName) {
    if (EffectNone::NAME == effectName) {
        changeEffect(new EffectNone());
    }
    else if (EffectMirror::NAME == effectName) {
        changeEffect(new EffectMirror());
    }
    else if (EffectInvisible::NAME == effectName) {
        changeEffect(new EffectInvisible());
    }
    else if (EffectReverse::NAME == effectName) {
        changeEffect(new EffectReverse());
    }
    else if (EffectZx::NAME == effectName) {
        changeEffect(new EffectZx());
    }
    else {
        ExInfo error = ExInfo("unknown view effect")
            .addInfo("effect", effectName);
        LOG_WARNING(error);
    }
}

//-----------------------------------------------------------------
static std::string
encode(const std::string &input)
{
    std::string output = input;
    StringTool::replace(output, "&", "&amp;");
    StringTool::replace(output, ",", "&comma;");
    return output;
}
//-----------------------------------------------------------------
static std::string
encode(int value)
{
    return StringTool::toString(value);
}
//-----------------------------------------------------------------
static std::string
decode(const std::string &input)
{
    std::string output = input;
    StringTool::replace(output, "&comma;", ",");
    StringTool::replace(output, "&amp;", "&");
    return output;
}
//-----------------------------------------------------------------
static int
decodeInt(const std::string &input)
{
    bool ok;
    int result = StringTool::readInt(input.c_str(), &ok);
    if (!ok) {
        LOG_WARNING(ExInfo("invalid int")
                .addInfo("input", input));
    }
    return result;
}
//-----------------------------------------------------------------
std::string
Anim::getState() const
{
    std::string output;
    output += encode(m_effect->getName());
    output += "," + encode(m_viewShift.getX());
    output += "," + encode(m_viewShift.getY());
    output += "," + encode(m_animName);
    output += "," + encode(m_animPhase);
    output += "," + encode(m_run);
    output += "," + encode(m_specialAnimName);
    output += "," + encode(m_specialAnimPhase);
    return output;
}
//-----------------------------------------------------------------
void
Anim::restoreState(const std::string &state)
{
    StringTool::t_args values = StringTool::split(state, ',');
    if (values.size() != 8) {
        LOG_WARNING(ExInfo("invalid anim state")
                .addInfo("state", state));
        return;
    }

    int i = 0;
    std::string effectName = decode(values[i++]);
    int x = decodeInt(values[i++]);
    int y = decodeInt(values[i++]);
    m_animName = decode(values[i++]);
    m_animPhase = decodeInt(values[i++]);
    m_run = decodeInt(values[i++]);
    m_specialAnimName = decode(values[i++]);
    m_specialAnimPhase = decodeInt(values[i++]);

    setEffect(effectName);
    m_viewShift = V2(x, y);
}
