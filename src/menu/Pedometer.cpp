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
#include "Level.h"

//-----------------------------------------------------------------
Pedometer::Pedometer(LevelStatus *status, Level *level)
{
    deactivate();
    m_level = level;
    m_status = status;
    m_solution = m_status->readSolvedMoves();

    prepareBg();
    prepareRack();

    m_numbers = ResImagePack::loadImage(
            Path::dataReadPath("images/menu/numbers.png"));

    takeHandler(new PedoInput(this));
}
//-----------------------------------------------------------------
Pedometer::~Pedometer()
{
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
    //TODO: allow to set differect images
    SDL_Surface *bgSurface = ResImagePack::loadImage(
            Path::dataReadPath("images/menu/mapa-0.png"));
    NodeDrawer drawer;
    drawer.setScreen(bgSurface);
    drawer.drawSelected(m_status->getLevelName());
    m_bg = new Picture(bgSurface, V2(0, 0));
    m_bg->deactivate();
}
//-----------------------------------------------------------------
    void
Pedometer::prepareRack()
{
    static const int POS_X = 193;
    static const int POS_Y = 141;

    //TODO: allow to set differect images
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
    m_rack->deactivate();
}

//-----------------------------------------------------------------
/**
 * Display menu and play menu music.
 */
    void
Pedometer::own_initState()
{
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
 * Hide picture.
 */
    void
Pedometer::own_pauseState()
{
    deactivate();
    m_rack->deactivate();
    m_bg->deactivate();
}
//-----------------------------------------------------------------
/**
 * Display picture.
 */
    void
Pedometer::own_resumeState()
{
    //NOTE: order is significant
    m_bg->activate();
    m_rack->activate();
    activate();
}

//-----------------------------------------------------------------
/**
 * Mark pixel under cursor as ative mask.
 */
    void
Pedometer::watchCursor()
{
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    m_activeMask = m_rack->getMaskAtWorld(V2(x, y));
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
    m_manager->changeState(m_level);
}
//-----------------------------------------------------------------
    void
Pedometer::runReplay()
{
    m_manager->changeState(m_level);
    m_level->loadReplay(m_solution);
}

//-----------------------------------------------------------------
    void
Pedometer::draw()
{
    drawNumbers(m_solution.size());
}
//-----------------------------------------------------------------
    void
Pedometer::drawNumbers(int value)
{
    static const int CIPHERS = 5;
    static const int POS_X = 275;
    static const int POS_Y = 177;

    int numberWidth = m_numbers->w;
    int numberHeight = m_numbers->h / 10;

    for (int i = CIPHERS - 1; i >= 0; --i) {
        int cipher = value % 10;
        value /= 10;
        int shiftY = numberHeight * (9 - cipher);
        int x = POS_X + numberWidth * i;

        drawNumber(x, POS_Y, shiftY);
    }
}
//-----------------------------------------------------------------
void
Pedometer::drawNumber(int x, int y, int shiftY)
{
    SDL_Rect dest_rect;
    dest_rect.x = x;
    dest_rect.y = y;

    SDL_Rect src_rect;
    src_rect.x = 0;
    src_rect.y = shiftY;
    src_rect.w = m_numbers->w;
    src_rect.h = m_numbers->h / 10;

    SDL_BlitSurface(m_numbers, &src_rect, m_screen, &dest_rect);
}

