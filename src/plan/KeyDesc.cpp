/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "KeyDesc.h"

//-----------------------------------------------------------------
/**
 * Create new key description.
 * @param index key index should be unique in one GameState
 * @param desc text description
 */
KeyDesc::KeyDesc(int index, const std::string &desc)
    : m_desc(desc)
{
    m_index = index;
}

