/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "StringTool.h"

#include <sstream>

//-----------------------------------------------------------------
/**
 * Convert string to number.
 * @param strNum string
 * @param ok true on sucess
 * @return number or zero
 */
    long
StringTool::readInt(const char *strNum, bool *ok)
{
    long result = 0;
    *ok = false;

    if (strNum != NULL) {
        char *endptr;
        result = strtol(strNum, &endptr, 0);
        if (strNum[0] != '\0' && endptr[0] == '\0') {
            *ok = true;
        }
    }

    if (!ok) {
        result = 0;
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Convert long to string
 * @return string value
 */
    std::string
StringTool::toString(long value)
{
    std::ostringstream buffer;
    buffer << value;
    return buffer.str();
}

