/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LevelDesc.h"

//-----------------------------------------------------------------
LevelDesc::LevelDesc(const std::string &lang,
                const std::string &levelname,
                const std::string &desc)
: Dialog(lang, "", desc)
{
    m_levelname = levelname;
}

