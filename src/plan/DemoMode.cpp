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

#include "demo-script.h"

//-----------------------------------------------------------------
DemoMode::DemoMode()
{
    m_bg = NULL;
    m_display = NULL;
    m_script->registerFunc("demo_display", script_demo_display);
    takeHandler(new DemoInput(this));
}
//-----------------------------------------------------------------
DemoMode::~DemoMode()
{
    own_cleanState();
}
//-----------------------------------------------------------------
    void
DemoMode::runDemo(Picture *new_bg, const Path &demoscript)
{
    if (m_bg) {
        delete m_bg;
    }
    m_bg = new_bg;
    m_script->doFile(demoscript);
}

//-----------------------------------------------------------------
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
    if (m_bg) {
        delete m_bg;
        m_bg = NULL;
    }
    if (m_display) {
        delete m_display;
        m_display = NULL;
    }

    interruptPlan();
    DialogAgent::agent()->killTalks();
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


