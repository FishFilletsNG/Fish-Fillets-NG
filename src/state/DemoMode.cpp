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
#include "DialogAgent.h"
#include "SubTitleAgent.h"
#include "SurfaceTool.h"

#include "demo-script.h"

//-----------------------------------------------------------------
DemoMode::DemoMode(const Path &demoscript)
    : m_demoscript(demoscript)
{
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
    killPlan();
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
    //NOTE: loaded dialogs will be keeped until room end
    if (m_surfaceBuffer) {
        SDL_FreeSurface(m_surfaceBuffer);
        m_surfaceBuffer = NULL;
    }
    if (m_display) {
        delete m_display;
        m_display = NULL;
    }

    killPlan();
}

//-----------------------------------------------------------------
void
DemoMode::killPlan()
{
    DialogAgent::agent()->killTalks();
    SubTitleAgent::agent()->killTalks();
    interruptPlan();
}
//-----------------------------------------------------------------
/**
 * Store picture to draw it.
 */
    bool
DemoMode::action_display(Picture *picture)
{
    if (m_display) {
        delete m_display;
    }
    m_display = picture;
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



