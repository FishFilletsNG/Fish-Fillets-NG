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
#include "Actor.h"
#include "ScriptState.h"
#include "DialogAgent.h"

#include "demo-script.h"

//-----------------------------------------------------------------
DemoMode::DemoMode()
{
    m_bg = NULL;
    m_display = NULL;
    m_script->registerFunc("demo_display", script_demo_display);
}
//-----------------------------------------------------------------
DemoMode::~DemoMode()
{
    //NOTE: loaded dialogs will be keeped until room end
    if (m_bg) {
        delete m_bg;
    }
    if (m_display) {
        delete m_display;
    }

    DialogAgent::agent()->killPlan();
    DialogAgent::agent()->killTalk();
    t_actors::iterator end = m_actors.end();
    for (t_actors::iterator i = m_actors.begin(); i != end; ++i) {
        delete i->second;
    }
}
//-----------------------------------------------------------------
void
DemoMode::runDemo(Picture *bg, const Path &demoscript)
{
    if (m_bg) {
        delete m_bg;
    }
    m_bg = bg;
    m_script->doFile(demoscript);
}
//-----------------------------------------------------------------
/**
 * Return existing or create new actor.
 */
Actor *
DemoMode::getActor(int model_index)
{
    Actor *result = NULL;
    t_actors::iterator it = m_actors.find(model_index);
    if (m_actors.end() == it) {
        result = new Actor();
        result->setIndex(model_index);
        m_actors[model_index] = result;
    }
    else {
        result = it->second;
    }
    return result;
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


