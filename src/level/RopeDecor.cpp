/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "RopeDecor.h"

#include "Cube.h"

#include "View.h"

#include "SDL_gfxPrimitives.h"

//-----------------------------------------------------------------
RopeDecor::RopeDecor(const Cube *model1, const Cube *model2,
        const V2 &shift1, const V2 &shift2)
: m_shift1(shift1), m_shift2(shift2)
{
    m_model1 = model1;
    m_model2 = model2;
}
//-----------------------------------------------------------------
/**
 * Draw line from one model to second model.
 */
void
RopeDecor::drawOnScreen(const View *view, SDL_Surface *screen)
{
    V2 loc1 = view->getScreenPos(m_model1).plus(m_shift1);
    V2 loc2 = view->getScreenPos(m_model2).plus(m_shift2);

    //NOTE: steel color
    Uint32 colorRGBA = 0x30404eff;
    lineColor(screen, loc1.getX(), loc1.getY(),
            loc2.getX(), loc2.getY(), colorRGBA);
}

