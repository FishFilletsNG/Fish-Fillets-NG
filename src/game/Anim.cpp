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
{
    m_animPack[SIDE_LEFT] = new ResImagePack();
    m_animPack[SIDE_RIGHT] = new ResImagePack();

    m_animName = "";
    m_animPhase = 0;
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
 * Draw one anim phase at screen position.
 */
    void
Anim::drawAt(SDL_Surface *screen, int x, int y, eSide side)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;

    SDL_Surface *surface =
        m_animPack[side]->getRes(m_animName, m_animPhase);

    SDL_BlitSurface(surface, NULL, screen, &rect);
    m_animPhase++;
    if (m_animPhase >= m_animPack[side]->countRes(m_animName)) {
        m_animPhase = 0;
    }
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
 * Set visage.
 * Set actual animation.
 */
    void
Anim::setAnim(const std::string &name, int phase)
{
    if (m_animName != name) {
        m_animPhase = phase;
    }
    m_animName = name;
}

