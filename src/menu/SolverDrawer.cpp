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
#include "LevelStatus.h"
#include "WiPara.h"
#include "BaseException.h"

//-----------------------------------------------------------------
/**
 * Prepares to draw info about best solver.
 * @param status shared level status
 */
SolverDrawer::SolverDrawer(LevelStatus *status)
{
    try {
        Font usedFont(Path::dataReadPath("font/font_menu.ttf"), 14);
        SDL_Color usedColor = {255, 255, 255, 255};

        Labels labels(Path::dataReadPath("script/labels.lua"));
        const char *labelName;
        switch (status->compareToBest()) {
            case 1:
                labelName = "solver_better";
                break;
            case 0:
                labelName = "solver_equals";
                break;
            default:
                labelName = "solver_worse";
        }
        StringTool::t_args args;
        args.push_back("");
        args.push_back(StringTool::toString(status->getBestMoves()));
        args.push_back(status->getBestAuthor());

        WiPara *para = new WiPara(
                labels.getFormatedLabel(labelName, args),
                usedFont, usedColor);
        para->enableCentered();
        para->recenter();
        addWidget(para);
    }
    catch (BaseException &e) {
        LOG_WARNING(e.info());
    }
}

