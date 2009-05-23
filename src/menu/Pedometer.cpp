/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Pedometer.h"

#include "LevelStatus.h"
#include "Level.h"
#include "PedoInput.h"
#include "LayeredPicture.h"

#include "ResImagePack.h"
#include "StateManager.h"
#include "NodeDrawer.h"
#include "SolverDrawer.h"
#include "Level.h"
#include "minmax.h"
#include "StringMsg.h"
#include "UnknownMsgException.h"

//-----------------------------------------------------------------
Pedometer::Pedometer(LevelStatus *status, Level *new_level)
{
    m_level = new_level;
    m_status = status;
    m_solution = m_status->readSolvedMoves();
    m_meterPhase = 0;
    m_bg = NULL;

    prepareBg();
    prepareRack();

    m_numbers = ResImagePack::loadImage(
            Path::dataReadPath("images/menu/numbers.png"));

    takeHandler(new PedoInput(this));
    registerDrawable(m_bg);
    registerDrawable(m_rack);
    registerDrawable(this);
}
//-----------------------------------------------------------------
Pedometer::~Pedometer()
{
    if (m_level) {
        delete m_level;
    }
    SDL_FreeSurface(m_numbers);
    delete m_rack;
    delete m_bg;
}
//-----------------------------------------------------------------
/**
 * Draw level name on background.
 */
    void
Pedometer::prepareBg()
{
    SDL_Surface *bgSurface = ResImagePack::loadImage(
            Path::dataReadPath("images/menu/map.png"));
    NodeDrawer drawer;
    drawer.setScreen(bgSurface);
    drawer.drawSelected(m_level->getLevelName());

    SolverDrawer solver(m_status);
    solver.setShift(V2((bgSurface->w - solver.getW()) / 2,
                bgSurface->h - 150));
    solver.drawOn(bgSurface);

    if (m_bg) {
        m_bg->changePicture(bgSurface);
    }
    else {
        m_bg = new Picture(bgSurface, V2(0, 0));
    }
}
//-----------------------------------------------------------------
    void
Pedometer::prepareRack()
{
    static const int POS_X = 193;
    static const int POS_Y = 141;

    m_rack = new LayeredPicture(
            Path::dataReadPath("images/menu/pedometer.png"),
            V2(POS_X, POS_Y),
            Path::dataReadPath("images/menu/pedometer_lower.png"),
            Path::dataReadPath("images/menu/pedometer_mask.png"));

    //TODO: don't use numeric constants for coordinates
    m_maskRun = m_rack->getMaskAt(V2(86, 100));
    m_maskReplay = m_rack->getMaskAt(V2(128, 100));
    m_maskCancel = m_rack->getMaskAt(V2(170, 100));
    m_activeMask = m_rack->getNoMask();
}

//-----------------------------------------------------------------
/**
 * Display menu and play menu music.
 */
    void
Pedometer::own_initState()
{
    registerWatcher("lang");
    own_resumeState();
}
//-----------------------------------------------------------------
    void
Pedometer::own_updateState()
{
    watchCursor();
}

//-----------------------------------------------------------------
/**
 * Mark pixel under cursor as ative mask.
 */
    void
Pedometer::watchCursor()
{
    V2 mouseLoc = getInput()->getMouseLoc();
    m_activeMask = m_rack->getMaskAtWorld(mouseLoc);
    if (m_activeMask == m_maskRun
            || m_activeMask == m_maskReplay
            || m_activeMask == m_maskCancel)
    {
        m_rack->setActiveMask(m_activeMask);
    }
    else {
        m_rack->setNoActive();
    }
}
//-----------------------------------------------------------------
/**
 * Start selected button.
 */
    void
Pedometer::runSelected()
{
    if (m_activeMask == m_maskRun) {
        runLevel();
    }
    else if (m_activeMask == m_maskReplay) {
        runReplay();
    }
    else {
        quitState();
    }
}
//-----------------------------------------------------------------
    void
Pedometer::runLevel()
{
    Level *levelState = m_level;
    m_level = NULL;
    GameState *poster = m_status->createPoster();
    if (poster) {
        poster->setNextState(levelState);
        changeState(poster);
    }
    else {
        changeState(levelState);
    }
}
//-----------------------------------------------------------------
    void
Pedometer::runReplay()
{
    Level *levelState = m_level;
    m_level = NULL;
    changeState(levelState);
    levelState->loadReplay(m_solution);
}
//-----------------------------------------------------------------
    void
Pedometer::drawOn(SDL_Surface *screen)
{
    drawNumbers(screen, m_solution.size());
}
//-----------------------------------------------------------------
/**
 * Draw number of steps.
 * Draw nice rotating numbers.
 */
    void
Pedometer::drawNumbers(SDL_Surface *screen, int value)
{
    static const int CIPHERS = 5;
    static const int POS_X = 275;
    static const int POS_Y = 177;
    static const int SHIFT_SPEED = 8;

    int numberWidth = m_numbers->w;
    int numberHeight = m_numbers->h / 10;

    for (int i = CIPHERS - 1; i >= 0; --i) {
        int cipher = value % 10;
        value /= 10;
        int x = POS_X + numberWidth * i;
        int shiftY = max(numberHeight * (9 - cipher),
                numberHeight * 9 - SHIFT_SPEED * m_meterPhase);
        m_meterPhase++;

        drawNumber(screen, x, POS_Y, shiftY);
    }
}
//-----------------------------------------------------------------
void
Pedometer::drawNumber(SDL_Surface *screen, int x, int y, int shiftY)
{
    SDL_Rect dest_rect;
    dest_rect.x = x;
    dest_rect.y = y;

    SDL_Rect src_rect;
    src_rect.x = 0;
    src_rect.y = shiftY;
    src_rect.w = m_numbers->w;
    src_rect.h = m_numbers->h / 10;

    SDL_BlitSurface(m_numbers, &src_rect, screen, &dest_rect);
}
//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - param_changed(lang) ... refresh bg
 *
 * @throws UnknownMsgException
 */
    void
Pedometer::receiveString(const StringMsg *msg)
{
    if (msg->equalsName("param_changed")) {
        std::string param = msg->getValue();
        if ("lang" == param) {
            prepareBg();
        }
        else {
            throw UnknownMsgException(msg);
        }
    }
    else {
        throw UnknownMsgException(msg);
    }
}
