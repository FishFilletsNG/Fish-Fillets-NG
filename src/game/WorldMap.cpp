/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WorldMap.h"

#include "LevelNode.h"
#include "NodeDrawer.h"
#include "worldmap-script.h"

#include "Log.h"
#include "WorldWay.h"
#include "ResImageAgent.h"
#include "VideoAgent.h"
#include "OptionAgent.h"
#include "SoundAgent.h"
#include "LogicException.h"
#include "ResDialogPack.h"
#include "LevelDesc.h"
#include "ScriptState.h"

//-----------------------------------------------------------------
WorldMap::WorldMap(const Path &bg)
{
    VideoAgent::agent()->removeDrawer(this);
    m_active = false;
    m_selected = NULL;
    m_startNode = NULL;

    m_bg = ResImageAgent::agent()->loadImage(bg);
    m_drawer = new NodeDrawer();
    m_descPack = new ResDialogPack();
}
//-----------------------------------------------------------------
WorldMap::~WorldMap()
{
    if (m_startNode) {
        delete m_startNode;
    }
    SDL_FreeSurface(m_bg);
    m_descPack->removeAll();
    delete m_descPack;
    delete m_drawer;
}
//-----------------------------------------------------------------
void
WorldMap::addDesc(const std::string &codename, LevelDesc *desc)
{
    m_descPack->addRes(codename, desc);
}
//-----------------------------------------------------------------
/**
 * Read dots postions and level descriptions.
 * @throws LogicException when cannot parse data file
 */
void
WorldMap::initWay(const Path &way, const Path &descfile)
{
    m_startNode = WorldWay::createWay(way);
    if (NULL == m_startNode) {
        throw LogicException(ExInfo("cannot create world way")
                .addInfo("file", way.getNative()));
    }

    ScriptState script;
    script.registerFunc("worldmap_addDesc", script_worldmap_addDesc);
    script.doFile(descfile);
}
//-----------------------------------------------------------------
void
WorldMap::activate()
{
    if (!m_active) {
        m_active = true;
        VideoAgent::agent()->acceptDrawer(this);

        SoundAgent::agent()->playMusic(
                Path::dataReadPath("music/menu.ogg"), NULL);
        //TODO: set with and height in one step
        OptionAgent *options = OptionAgent::agent();
        options->setParam("caption", findDesc("menu"));
        options->setParam("screen_width", getW());
        options->setParam("screen_height", getH());
    }
}
//-----------------------------------------------------------------
void
WorldMap::deactivate()
{
    if (m_active) {
        m_active = false;
        VideoAgent::agent()->removeDrawer(this);
        SoundAgent::agent()->stopMusic();
    }
}
//-----------------------------------------------------------------
/**
 * Mark node under cursor as selected.
 */
void
WorldMap::watchCursor()
{
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    m_selected = m_startNode->findSelected(V2(x, y));
}
//-----------------------------------------------------------------
/**
 * Return selected level or NULL;
 */
Level *
WorldMap::createSelected() const
{
    Level *result = NULL;
    if (m_selected) {
        std::string title = findDesc(m_selected->getCodeName());
        title.append(": " + findLevelName(m_selected->getCodeName()));
        result = m_selected->createLevel(title);
    }
    return result;
}
//-----------------------------------------------------------------
void
WorldMap::markSolved()
{
    if (m_selected) {
        m_selected->setState(LevelNode::STATE_SOLVED);
    }
}
//-----------------------------------------------------------------
void
WorldMap::draw()
{
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    SDL_BlitSurface(m_bg, NULL, m_screen, &rect);

    m_drawer->setScreen(m_screen);
    m_startNode->drawPath(m_drawer);
    if (m_selected) {
        m_drawer->drawSelected(findLevelName(m_selected->getCodeName()));
    }
}
//-----------------------------------------------------------------
std::string
WorldMap::findLevelName(const std::string &codename) const
{
    std::string result;
    LevelDesc *desc =
        dynamic_cast<LevelDesc*>(m_descPack->findDialogHard(codename));
    if (desc) {
        result = desc->getLevelName();
    }
    else {
        result = codename;
    }
    return result;
}
//-----------------------------------------------------------------
std::string
WorldMap::findDesc(const std::string &codename) const
{
    std::string result;
    LevelDesc *desc =
        dynamic_cast<LevelDesc*>(m_descPack->findDialogHard(codename));
    if (desc) {
        result = desc->getDesc();
    }
    else {
        result = "???";
    }
    return result;
}

