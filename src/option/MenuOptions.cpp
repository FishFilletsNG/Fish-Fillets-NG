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
#include "HBox.h"
#include "VBox.h"
#include "WiPicture.h"
#include "WiSpace.h"
#include "WiButton.h"
#include "WiStatusBar.h"
#include "Slider.h"
#include "SelectLang.h"
#include "RadioBox.h"

#include "Font.h"
#include "Labels.h"
#include "SimpleMsg.h"
#include "OptionsInput.h"
#include "OptionAgent.h"
#include "SurfaceTool.h"
#include "StringMsg.h"
#include "UnknownMsgException.h"

//-----------------------------------------------------------------
MenuOptions::MenuOptions()
{
    m_container = NULL;
    m_statusBar = NULL;
    m_needRefresh = false;
    registerDrawable(this);
    prepareMenu();

    takeHandler(new OptionsInput(this));
}
//-----------------------------------------------------------------
MenuOptions::~MenuOptions()
{
    delete m_container;
}
//-----------------------------------------------------------------
/**
 * Display menu.
 */
    void
MenuOptions::own_initState()
{
    registerWatcher("lang");
    own_resumeState();
}
//-----------------------------------------------------------------
/**
 * Display options menu.
 * Menu will be centred on screen.
 */
    void
MenuOptions::own_resumeState()
{
    int contentW = m_container->getW();
    int contentH = m_container->getH();
    OptionAgent *options = OptionAgent::agent();
    int screenW = options->getAsInt("screen_width");
    int screenH = options->getAsInt("screen_height");

    m_container->setShift(
            V2((screenW - contentW) / 2, (screenH - contentH) / 2));
}
//-----------------------------------------------------------------
/**
 * Update statusbar.
 */
    void
MenuOptions::own_updateState()
{
    if (m_needRefresh) {
        m_needRefresh = false;
        prepareMenu();
        own_resumeState();
    }
    std::string tooltip = m_container->getTip(getInput()->getMouseLoc());
    m_statusBar->setLabel(tooltip);
}
//-----------------------------------------------------------------
void
MenuOptions::prepareMenu()
{
    if (m_container) {
        deregisterDrawable(m_container);
        delete m_container;
        m_container = NULL;
    }

    Labels labels(Path::dataReadPath("script/labels.lua"));
    IWidget *soundBox = createSoundPanel(labels);
    IWidget *musicBox = createMusicPanel(labels);

    VBox *vbox = new VBox();
    vbox->addWidget(soundBox);
    vbox->addWidget(new WiSpace(0, 10));
    vbox->addWidget(musicBox);
    vbox->addWidget(new WiSpace(0, 10));
    vbox->addWidget(createLangPanel(labels));
    vbox->addWidget(new WiSpace(0, 5));
    vbox->addWidget(createSpeechPanel(labels));
    vbox->addWidget(new WiSpace(0, 5));
    vbox->addWidget(createSubtitlesPanel(labels));

    IWidget *backButton = createBackButton(labels);
    m_statusBar = createStatusBar(musicBox->getW() - backButton->getW());
    HBox *backBox = new HBox();
    backBox->addWidget(m_statusBar);
    backBox->addWidget(backButton);

    vbox->addWidget(backBox);
    m_container = vbox;
    registerDrawable(m_container);
}
//-----------------------------------------------------------------
IWidget *
MenuOptions::createSoundPanel(const Labels &labels)
{
    HBox *soundBox = new HBox();
    soundBox->addWidget(new WiPicture(
                Path::dataReadPath("images/menu/volume_sound.png")));
    soundBox->addWidget(new WiSpace(10, 0));
    soundBox->addWidget(new Slider("volume_sound", 0, 100));
    soundBox->setTip(labels.getLabel("menu_sound"));
    return soundBox;
}
//-----------------------------------------------------------------
IWidget *
MenuOptions::createMusicPanel(const Labels &labels)
{
    HBox *musicBox = new HBox();
    musicBox->addWidget(new WiPicture(
                Path::dataReadPath("images/menu/volume_music.png")));
    musicBox->addWidget(new WiSpace(10, 0));
    musicBox->addWidget(new Slider("volume_music", 0, 100));
    musicBox->setTip(labels.getLabel("menu_music"));
    return musicBox;
}
//-----------------------------------------------------------------
IWidget *
MenuOptions::createLangPanel(const Labels &labels)
{
    HBox *langBox = new HBox();
    langBox->addWidget(new WiPicture(
                Path::dataReadPath("images/menu/lang.png")));
    langBox->addWidget(new WiSpace(10, 0));
    langBox->addWidget(new SelectLang("lang",
                Path::dataReadPath("script/select_lang.lua")));
    langBox->setTip(labels.getLabel("menu_lang"));
    return langBox;
}
//-----------------------------------------------------------------
/**
 * Creates menu to select 'speech'.
 * NOTE: default speech is customized in "script/init.lua"
 */
IWidget *
MenuOptions::createSpeechPanel(const Labels &labels)
{
    HBox *speechBox = new HBox();
    speechBox->addWidget(new WiPicture(
                Path::dataReadPath("images/menu/speech.png")));
    speechBox->addWidget(new WiSpace(10, 0));
    speechBox->addWidget(new SelectLang("speech",
            Path::dataReadPath("script/select_speech.lua")));
    speechBox->setTip(labels.getLabel("menu_speech"));
    return speechBox;
}
//-----------------------------------------------------------------
IWidget *
MenuOptions::createSubtitlesPanel(const Labels &labels)
{
    HBox *chooseBox = new HBox();
    chooseBox->addWidget(new WiPicture(
                Path::dataReadPath("images/menu/subtitle.png")));
    chooseBox->addWidget(new WiSpace(10, 0));
    chooseBox->addWidget(new RadioBox("subtitles", "1",
                Path::dataReadPath("images/menu/subtitles/yes.png")));
    chooseBox->addWidget(new RadioBox("subtitles", "0",
                Path::dataReadPath("images/menu/subtitles/no.png")));
    chooseBox->setTip(labels.getLabel("menu_subtitles"));
    return chooseBox;
}
//-----------------------------------------------------------------
IWidget *
MenuOptions::createBackButton(const Labels &labels)
{
    IWidget *button = new WiButton(
            new WiPicture(Path::dataReadPath("images/menu/back.png")),
            new SimpleMsg(this, "quit"));
    button->setTip(labels.getLabel("menu_back"));
    return button;
}
//-----------------------------------------------------------------
WiStatusBar *
MenuOptions::createStatusBar(int width)
{
    SDL_Color color = {0, 255, 0, 255};
    Font *new_font = new Font(Path::dataReadPath("font/font_menu.ttf"), 16);
    return new WiStatusBar(new_font, color, width);
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
}
//-----------------------------------------------------------------
void
MenuOptions::drawOn(SDL_Surface *screen)
{
    SDL_Color gray = {0xf0, 0xf0, 0xf0, 129};
    SurfaceTool::alphaFill(screen, NULL, gray);
}
//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - param_changed(lang) ... refresh menu
 *
 * @throws UnknownMsgException
 */
    void
MenuOptions::receiveString(const StringMsg *msg)
{
    if (msg->equalsName("param_changed")) {
        std::string param = msg->getValue();
        if ("lang" == param) {
            m_needRefresh = true;
        }
        else {
            throw UnknownMsgException(msg);
        }
    }
    else {
        throw UnknownMsgException(msg);
    }
}

