/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WiPara.h"

#include "WiLabel.h"
#include "StringTool.h"

//-----------------------------------------------------------------
WiPara::WiPara(const std::string &text, const Font &font,
        const SDL_Color &color)
{
    const std::string space = " ";
    StringTool::t_args lines = StringTool::split(text, '\n');
    for (unsigned int i = 0; i < lines.size(); ++i) {
        std::string line = lines[i];
        if (line.empty()) {
            line = space;
        }
        addWidget(new WiLabel(line, font, color));
    }
}
