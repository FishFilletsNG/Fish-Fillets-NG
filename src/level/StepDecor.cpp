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
    static const SDL_Color COLOR_ORANGE = {255, 197, 102, 255};
    static const SDL_Color COLOR_BLUE = {162, 244, 255, 255};

    if (OptionAgent::agent()->getAsBool("show_steps")) {
        SDL_Color color;
        if (m_counter->isPowerful()) {
            color = COLOR_BLUE;
        }
        else {
            color = COLOR_ORANGE;
        }

        std::string steps = StringTool::toString(m_counter->getStepCount());
        SDL_Surface *text_surface = m_font.renderTextOutlined(steps, color);

        SDL_Rect rect;
        rect.x = screen->w - text_surface->w;
        rect.y = 10;
        SDL_BlitSurface(text_surface, NULL, screen, &rect);
        SDL_FreeSurface(text_surface);
    }
}

