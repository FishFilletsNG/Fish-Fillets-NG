/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "StringTool.h"

#include <stdlib.h>
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
//-----------------------------------------------------------------
    bool
StringTool::startsWith(const std::string &str,
        const std::string &prefix)
{
    return prefix == str.substr(0, prefix.size());
}
//-----------------------------------------------------------------
/**
 * Replace one substring with another
 * @param buffer string to change
 * @param pattern what replace
 * @param newstring what to place
 */
    void
StringTool::replace(std::string &buffer,
        const std::string &pattern, const std::string &newstring)
{
    std::string::size_type pos = buffer.find(pattern);
    while (pos != std::string::npos) {
        buffer.replace(pos, pattern.size(), newstring);
        pos = buffer.find(pattern, pos + newstring.size());
    }
}
//-----------------------------------------------------------------
/**
 * Split string.
 * @return splited segments without separator
 */
    StringTool::t_args
StringTool::split(const std::string &str, char separator)
{
    std::string remain = str;
    t_args args;

    std::string::size_type pos = remain.find(separator);
    while (pos != std::string::npos) {
        args.push_back(remain.substr(0, pos));
        remain.erase(0, pos + 1);

        pos = remain.find(separator);
    }
    if (remain.size() > 0) {
        args.push_back(remain);
    }

    return args;
}

//-----------------------------------------------------------------
/**
 * Return number of Unicode entities inside given UTF-8 text.
 */
    int
StringTool::utf8Length(const std::string &str) {
    int length = 0;
    const char *p = str.c_str();
    char c;
    // How to recognize entity:
    // Unicode entities start with 0xxxxxxx or 11xxxxxx,
    // the other bytes start with 10xxxxxx.
    // (thanks to Egmont Koblinger)
    while ((c = *p++) != 0) {
        if ((c & 0xC0) != 0x80) {
            length++;
        }
    }
    return length;
}
