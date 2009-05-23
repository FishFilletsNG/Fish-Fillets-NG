/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "NodeDrawer.h"

#include "ResImagePack.h"
#include "Font.h"

#include "Log.h"
#include "V2.h"
#include "Path.h"
#include "LevelNode.h"
#include "TimerAgent.h"
#include "minmax.h"

#include "SDL_gfxPrimitives.h"

//-----------------------------------------------------------------
NodeDrawer::NodeDrawer()
{
    m_font = new Font(Path::dataReadPath("font/font_menu.ttf"), 22);

    m_imagePack = new ResImagePack();
    m_imagePack->addImage("solved",
            Path::dataReadPath("images/menu/n0.png"));

    m_imagePack->addImage("open",
            Path::dataReadPath("images/menu/n1.png"));
    m_imagePack->addImage("open",
            Path::dataReadPath("images/menu/n2.png"));
    m_imagePack->addImage("open",
            Path::dataReadPath("images/menu/n3.png"));
    m_imagePack->addImage("open",
            Path::dataReadPath("images/menu/n4.png"));

    m_imagePack->addImage("far",
            Path::dataReadPath("images/menu/n_far.png"));
}
//-----------------------------------------------------------------
NodeDrawer::~NodeDrawer()
{
    m_imagePack->removeAll();
    delete m_imagePack;
    delete m_font;
}
//-----------------------------------------------------------------
/**
 * Draw blinking dot centred on node position.
 */
void
NodeDrawer::drawNode(const LevelNode *node) const
{
    V2 loc = node->getLoc();
    drawDot(m_imagePack->getRes("far"), loc);

    SDL_Surface *dot = NULL;
    switch (node->getState()) {
        case LevelNode::STATE_FAR:
            return;
        case LevelNode::STATE_OPEN:
            {
                int phase = TimerAgent::agent()->getCycles() % 10;
                if (phase > 4) {
                    phase--;
                }
                if (phase > 7) {
                    phase--;
                }
                if (phase >= 4) {
                    phase = 7 - phase;
                }
                dot = m_imagePack->getRes("open", phase);
            }
            break;
        case LevelNode::STATE_SOLVED:
            dot = m_imagePack->getRes("solved");
            break;
        default:
            LOG_WARNING(ExInfo("don't know how to draw node")
                    .addInfo("state", node->getState()));
            return;
    }
    drawDot(dot, loc);
}
//-----------------------------------------------------------------
/**
 * Draw centred.
 * @param x x cord. or centre
 * @param x y cord. or centre
 */
void
NodeDrawer::drawDot(SDL_Surface *dot, const V2 &loc) const
{
    SDL_Rect rect;
    rect.x = loc.getX() - dot->w / 2;
    rect.y = loc.getY() - dot->h / 2;
    SDL_BlitSurface(dot, NULL, m_screen, &rect);
}
//-----------------------------------------------------------------
/**
 * Highlightes selected node.
 */
void
NodeDrawer::drawSelect(const V2 &loc) const
{

    const SDL_Surface *dot = m_imagePack->getRes("solved");
    int radius = max(dot->w, dot->h) / 2 + 1;
    Uint32 colorRGBA = 0xffc61880;

    filledCircleColor(m_screen, loc.getX(), loc.getY(), radius, colorRGBA);
}
//-----------------------------------------------------------------
/**
 * Draws name of selected level.
 */
void
NodeDrawer::drawSelected(const std::string &levelname) const
{
    //TODO: draw deflected text
    int text_width = m_font->calcTextWidth(levelname);

    SDL_Rect rect;
    rect.x = (m_screen->w - text_width) / 2;
    rect.y = m_screen->h - 50;

    SDL_Color color = {255, 255, 0, 255};
    SDL_Surface *surface = m_font->renderTextOutlined(levelname, color);
    SDL_BlitSurface(surface, NULL, m_screen, &rect);
    SDL_FreeSurface(surface);
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

    Uint32 colorRGBA = 0xffff00ff;
    aalineColor(m_screen, x1, y1, x2, y2, colorRGBA);
    aalineColor(m_screen, x1 - 1, y1 - 1 , x2 - 1, y2 - 1, colorRGBA);
    aalineColor(m_screen, x1 + 1, y1 + 1 , x2 + 1, y2 + 1, colorRGBA);
    aalineColor(m_screen, x1 - 1, y1 + 1 , x2 - 1, y2 + 1, colorRGBA);
    aalineColor(m_screen, x1 + 1, y1 - 1 , x2 + 1, y2 - 1, colorRGBA);
}

