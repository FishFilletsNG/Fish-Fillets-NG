/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "IntMsg.h"

#include "StringTool.h"

//-----------------------------------------------------------------
std::string
IntMsg::toString() const
{
    std::string result = m_listenerName;
    result.append("->" + m_name);
    result.append("(" + StringTool::toString(m_value) + ")");
    return result;
}
