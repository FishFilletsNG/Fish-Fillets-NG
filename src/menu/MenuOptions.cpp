/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "MenuOptions.h"

#include "Path.h"
#include "SelectLang.h"
#include "OptionsInput.h"
#include "OptionAgent.h"
#include "VideoAgent.h"
#include "minmax.h"

//-----------------------------------------------------------------
MenuOptions::MenuOptions()
{
    m_selectLang = new SelectLang(
            Path::dataReadPath("script/select_lang.lua"));
    m_selectLang->initFlags();
    m_selectLang->deactivate();
    takeHandler(new OptionsInput(this));
}
//-----------------------------------------------------------------
MenuOptions::~MenuOptions()
{
    delete m_selectLang;
}
//-----------------------------------------------------------------
/**
 * Display menu.
 */
    void
MenuOptions::own_initState()
{
    own_resumeState();
}
//-----------------------------------------------------------------
    void
MenuOptions::own_updateState()
{
    //TODO: implement volume controls
}
//-----------------------------------------------------------------
/**
 * Hide menu.
 */
    void
MenuOptions::own_pauseState()
{
    m_selectLang->deactivate();
}
//-----------------------------------------------------------------
/**
 * Display buttons to select lang.
 * Buttons will be centred on screen.
 */
    void
MenuOptions::own_resumeState()
{
    m_selectLang->activate();

    OptionAgent *options = OptionAgent::agent();
    int origW = options->getAsInt("screen_width");
    int origH = options->getAsInt("screen_height");
    int requiredW = m_selectLang->getW();
    int requiredH = m_selectLang->getH();
    options->setParam("screen_width", max(origW, requiredW));
    options->setParam("screen_height", max(origH, requiredH));
    VideoAgent::agent()->initVideoMode();

    m_selectLang->setShift(
            V2((origW - requiredW) / 2, (origH - requiredH) / 2));
}
//-----------------------------------------------------------------
/**
 * Start level under pressed button.
 * Start pedometer when level is solved already.
 */
    void
MenuOptions::mouseButton(const MouseStroke &stroke)
{
    m_selectLang->mouseButton(stroke);
    quitState();
}

