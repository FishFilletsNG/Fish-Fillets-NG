/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "StepDecor.h"

#include "StepCounter.h"

#include "Path.h"
#include "StringTool.h"
#include "OptionAgent.h"

//-----------------------------------------------------------------
StepDecor::StepDecor(const StepCounter *counter)
    : m_font(Path::dataReadPath("font/font_console.ttf"), 20)
{
    m_counter = counter;
}
//-----------------------------------------------------------------
/**
 * Draw number of steps in right-top corner.
 */
void
StepDecor::drawOnScreen(const View * /*view*/, SDL_Surface *screen)
{
    if (OptionAgent::agent()->getAsInt("show_steps")) {
        SDL_Color white = {255, 255, 255, 255};
        std::string steps = StringTool::toString(m_counter->getStepCount());
        int text_width = m_font.calcTextWidth(steps);
        SDL_Surface *text_surface = m_font.renderTextOutlined(steps, white);

        SDL_Rect rect;
        rect.x = screen->w - text_width - 10;
        rect.y = 10;
        SDL_BlitSurface(text_surface, NULL, screen, &rect);
        SDL_FreeSurface(text_surface);
    }
}

