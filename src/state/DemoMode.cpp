/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "DemoMode.h"

#include "Picture.h"
#include "StateManager.h"
#include "DemoInput.h"

#include "ScriptState.h"
#include "SubTitleAgent.h"
#include "OptionAgent.h"
#include "VideoAgent.h"
#include "SurfaceTool.h"

#include "demo-script.h"

//-----------------------------------------------------------------
DemoMode::DemoMode(const Path &demoscript)
    : m_demoscript(demoscript)
{
    m_oldLimitY = 0;
    m_display = NULL;
    m_surfaceBuffer = NULL;
    m_script->registerFunc("demo_display", script_demo_display);
    takeHandler(new DemoInput(this));
    registerDrawable(this);
    registerDrawable(SubTitleAgent::agent());
}
//-----------------------------------------------------------------
DemoMode::~DemoMode()
{
    own_cleanState();
}
//-----------------------------------------------------------------
/**
 * Run demo.
 */
    void
DemoMode::own_initState()
{
    m_oldLimitY = SubTitleAgent::agent()->getLimitY();
    m_script->doFile(m_demoscript);
}
//-----------------------------------------------------------------
/**
 * Execute next demo command.
 */
void
DemoMode::own_updateState()
{
    if (satisfyPlan()) {
        quitState();
    }
}
//-----------------------------------------------------------------
void
DemoMode::own_cleanState()
{
    //NOTE: loaded dialogs are released by ~Planner()
    if (m_surfaceBuffer) {
        SDL_FreeSurface(m_surfaceBuffer);
        m_surfaceBuffer = NULL;
    }
    if (m_display) {
        delete m_display;
        m_display = NULL;
    }

    SubTitleAgent::agent()->setLimitY(m_oldLimitY);
    killPlan();
}

//-----------------------------------------------------------------
/**
 * Store picture to draw it.
 * NOTE: limitY for long subtitles are prepared when display is set
 * before planning start
 */
    bool
DemoMode::action_display(Picture *picture)
{
    if (m_display) {
        delete m_display;
    }
    m_display = picture;

    if (NULL == m_surfaceBuffer) {
        OptionAgent *options = OptionAgent::agent();
        options->setParam("screen_width", m_display->getW());
        options->setParam("screen_height", m_display->getH());
        VideoAgent::agent()->initVideoMode();

        SubTitleAgent::agent()->setLimitY(2 * m_display->getH());
    }
    return true;
}
//-----------------------------------------------------------------
void
DemoMode::drawOn(SDL_Surface *screen)
{
    if (NULL == m_surfaceBuffer) {
        m_surfaceBuffer = SurfaceTool::createEmpty(screen);
    }

    if (m_display) {
        m_display->drawOn(m_surfaceBuffer);
    }
    SDL_BlitSurface(m_surfaceBuffer, NULL, screen, NULL);
}



