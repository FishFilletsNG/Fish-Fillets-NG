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

        if (status->getBestMoves() > 0) {
            addWidget(new WiLabel(labels.getLabel("solver_title"),
                        usedFont, usedColor));

            StringTool::t_args args;
            args.push_back("");
            args.push_back(StringTool::toString(status->getBestMoves()));
            args.push_back(status->getBestAuthor());
            addWidget(new WiLabel(labels.getFormatedLabel("solver_best", args),
                        usedFont, usedColor));
        }
        if (status->isBetter()) {
            addWidget(new WiLabel(labels.getLabel("solver_check"),
                        usedFont, usedColor));
        }

        enableCentered();
        recenter();
    }
    catch (BaseException &e) {
        LOG_WARNING(e.info());
    }
}

