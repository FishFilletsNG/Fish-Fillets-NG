/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "NodeDrawer.h"

#include "Log.h"
#include "LevelNode.h"
#include "ResImageAgent.h"
#include "ResFontPack.h"
#include "Path.h"

#include "SDL_gfxPrimitives.h"

//-----------------------------------------------------------------
NodeDrawer::NodeDrawer()
{
    //TODO: blinking dots
    m_dotFar = ResImageAgent::agent()->loadImage(
            Path::dataReadPath("images/menu/n1.png"));
    m_dotOpen = ResImageAgent::agent()->loadImage(
            Path::dataReadPath("images/menu/n3.png"));
    m_dotSolved = ResImageAgent::agent()->loadImage(
            Path::dataReadPath("images/menu/n0.png"));
    m_dotSelected = ResImageAgent::agent()->loadImage(
            Path::dataReadPath("images/menu/n4.png"));
    m_dotRadius = m_dotOpen->w / 2;

    m_font = ResFontPack::loadFont(Path::dataReadPath("font/font_menu.png"));
}
//-----------------------------------------------------------------
NodeDrawer::~NodeDrawer()
{
    SDL_FreeSurface(m_dotFar);
    SDL_FreeSurface(m_dotOpen);
    SDL_FreeSurface(m_dotSolved);
    SDL_FreeSurface(m_dotSelected);
    SFont_FreeFont(m_font);
}
//-----------------------------------------------------------------
void
NodeDrawer::drawNode(const LevelNode *node) const
{
    V2 loc = node->getLoc();
    SDL_Rect rect;
    rect.x = loc.getX() - m_dotRadius;
    rect.y = loc.getY() - m_dotRadius;

    SDL_Surface *dot = NULL;
    switch (node->getState()) {
        case LevelNode::STATE_FAR:
            dot = m_dotFar;
            break;
        case LevelNode::STATE_OPEN:
            dot = m_dotOpen;
            break;
        case LevelNode::STATE_SOLVED:
            dot = m_dotSolved;
            break;
        default:
            LOG_WARNING(ExInfo("don't know how to draw node")
                    .addInfo("state", node->getState()));
            return;
    }
    SDL_BlitSurface(dot, NULL, m_screen, &rect);
}
//-----------------------------------------------------------------
void
NodeDrawer::drawSelected(const std::string &levelname) const
{
    //TODO: draw deflected text
    int y = m_screen->h - 50;
    SFont_WriteCenter(m_screen, m_font, y, levelname.c_str());
}
//-----------------------------------------------------------------
void
NodeDrawer::drawEdge(const LevelNode *start, const LevelNode *end) const
{
    //TODO: nice curves
    Sint16 x1 = start->getLoc().getX();
    Sint16 y1 = start->getLoc().getY();
    Sint16 x2 = end->getLoc().getX();
    Sint16 y2 = end->getLoc().getY();
    aalineRGBA(m_screen, x1, y1, x2, y2, 255, 255, 0, 255);
    aalineRGBA(m_screen, x1 - 1, y1 - 1 , x2 - 1, y2 - 1, 255, 255, 0, 255);
    aalineRGBA(m_screen, x1 + 1, y1 + 1 , x2 + 1, y2 + 1, 255, 255, 0, 255);
    aalineRGBA(m_screen, x1 - 1, y1 + 1 , x2 - 1, y2 + 1, 255, 255, 0, 255);
    aalineRGBA(m_screen, x1 + 1, y1 - 1 , x2 + 1, y2 - 1, 255, 255, 0, 255);
}

