/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SolverDrawer.h"

#include "Log.h"
#include "Path.h"
#include "Font.h"
#include "Labels.h"
#include "StringTool.h"
#include "LevelStatus.h"
#include "WiLabel.h"
#include "BaseException.h"

//-----------------------------------------------------------------
/**
 * Prepares to draw info about best solver.
 * @param status shared level status
 */
SolverDrawer::SolverDrawer(LevelStatus *status)
{
    try {
        Labels labels(Path::dataReadPath("script/labels.lua"));

        Font usedFont(Path::dataReadPath("font/font_menu.ttf"), 14);
        SDL_Color usedColor = {255, 255, 255, 255};

        StringTool::t_args args;
        args.push_back("");
        args.push_back(StringTool::toString(status->getBestMoves()));
        args.push_back(status->getBestAuthor());

        std::string label1;
        std::string label2;
        if (status->isBetter()) {
            label1 = labels.getFormatedLabel("solver_better1", args);
            label2 = labels.getFormatedLabel("solver_better2", args);
        }
        else {
            label1 = labels.getFormatedLabel("solver_worse1", args);
            label2 = labels.getFormatedLabel("solver_worse2", args);
        }

        addWidget(new WiLabel(label1, usedFont, usedColor));
        addWidget(new WiLabel(label2, usedFont, usedColor));
        enableCentered();
        recenter();
    }
    catch (BaseException &e) {
        LOG_WARNING(e.info());
    }
}

