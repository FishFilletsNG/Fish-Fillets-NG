/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WiLabel.h"

#include "Font.h"

//-----------------------------------------------------------------
WiLabel::WiLabel(const std::string &text, const Font &font,
        const SDL_Color &color)
    : WiPicture(font.renderTextOutlined(text, color))
{
}
