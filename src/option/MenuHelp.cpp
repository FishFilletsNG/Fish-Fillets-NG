/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "MenuHelp.h"

#include "Log.h"
#include "Path.h"
#include "VBox.h"
#include "WiLabel.h"

#include "Labels.h"
#include "Font.h"
#include "HelpInput.h"
#include "OptionAgent.h"
#include "SurfaceTool.h"

//-----------------------------------------------------------------
MenuHelp::MenuHelp()
{
    m_help = new VBox();
    prepareText();

    takeHandler(new HelpInput(this));
    registerDrawable(this);
    registerDrawable(m_help);
}
//-----------------------------------------------------------------
MenuHelp::~MenuHelp()
{
    delete m_help;
}
//-----------------------------------------------------------------
/**
 * Display help centerd on screen.
 */
    void
MenuHelp::own_initState()
{
    own_resumeState();
}
//-----------------------------------------------------------------
/**
 * Help will be centred on screen.
 */
    void
MenuHelp::own_resumeState()
{
    int contentW = m_help->getW();
    int contentH = m_help->getH();
    OptionAgent *options = OptionAgent::agent();
    int screenW = options->getAsInt("screen_width");
    int screenH = options->getAsInt("screen_height");

    m_help->setShift(
            V2((screenW - contentW) / 2, (screenH - contentH) / 2));
}
//-----------------------------------------------------------------
void
MenuHelp::prepareText()
{
    Labels labels(Path::dataReadPath("script/labels.lua"));
    std::string text = labels.getLabel("help");

    Font usedFont(Path::dataReadPath("font/font_menu.ttf"), 14);
    SDL_Color usedColor = {255, 255, 255, 255};

    std::string space = " ";
    StringTool::t_args lines = StringTool::split(text, '\n');
    for (unsigned int i = 0; i < lines.size(); ++i) {
        const std::string *line = &(lines[i]);
        if (line->empty()) {
            line = &space;
        }
        m_help->addWidget(new WiLabel(*line, usedFont, usedColor));
    }
}
//-----------------------------------------------------------------
void
MenuHelp::drawOn(SDL_Surface *screen)
{
    SDL_Color gray = {0x00, 0x00, 0x00, 128};
    SurfaceTool::alphaFill(screen, NULL, gray);
}

