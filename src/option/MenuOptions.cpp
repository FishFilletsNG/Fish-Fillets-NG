/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "MenuOptions.h"

#include "Picture.h"
#include "WidgetContainer.h"

#include "Path.h"
#include "HBox.h"
#include "VBox.h"
#include "WiPicture.h"
#include "WiSpace.h"
#include "Slider.h"
#include "SelectLang.h"
#include "OptionsInput.h"
#include "OptionAgent.h"
#include "VideoAgent.h"

//-----------------------------------------------------------------
MenuOptions::MenuOptions()
{
    m_bg = new Picture(Path::dataReadPath("images/menu/map.png"), V2(0, 0));
    m_bg->deactivate();

    HBox *soundBox = new HBox();
    soundBox->addWidget(new WiPicture(
                Path::dataReadPath("images/menu/volume_sound.png")));
    soundBox->addWidget(new WiSpace(10, 0));
    soundBox->addWidget(new Slider("volume_sound", 0, 100));

    HBox *musicBox = new HBox();
    musicBox->addWidget(new WiPicture(
                Path::dataReadPath("images/menu/volume_music.png")));
    musicBox->addWidget(new WiSpace(10, 0));
    musicBox->addWidget(new Slider("volume_music", 0, 100));

    VBox *vbox = new VBox();
    vbox->addWidget(soundBox);
    vbox->addWidget(new WiSpace(0, 10));
    vbox->addWidget(musicBox);
    vbox->addWidget(new WiSpace(0, 10));
    vbox->addWidget(new SelectLang(
                Path::dataReadPath("script/select_lang.lua")));
    m_container = new WidgetContainer(vbox);
    m_container->deactivate();

    takeHandler(new OptionsInput(this));
}
//-----------------------------------------------------------------
MenuOptions::~MenuOptions()
{
    delete m_container;
    delete m_bg;
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
    m_container->deactivate();
    m_bg->deactivate();
}
//-----------------------------------------------------------------
/**
 * Display options menu.
 * Menu will be centred on screen.
 */
    void
MenuOptions::own_resumeState()
{
    m_bg->activate();
    m_container->activate();

    int bgW = m_bg->getW();
    int bgH = m_bg->getH();
    OptionAgent *options = OptionAgent::agent();
    options->setParam("screen_width", bgW);
    options->setParam("screen_height", bgH);
    VideoAgent::agent()->initVideoMode();

    int contentW = m_container->getW();
    int contentH = m_container->getH();
    m_container->setShift(
            V2((bgW - contentW) / 2, (bgH - contentH) / 2));
}
//-----------------------------------------------------------------
/**
 * Start level under pressed button.
 * Start pedometer when level is solved already.
 */
    void
MenuOptions::mouseButton(const MouseStroke &stroke)
{
    m_container->mouseButton(stroke);
    //TEST: don't quit
    //quitState();
}

