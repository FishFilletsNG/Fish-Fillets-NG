/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ExInfo.h"

#include "StringTool.h"

//-----------------------------------------------------------------
/**
 * Info about problem.
 * @param problem short info
 */
    ExInfo::ExInfo(const std::string &problem) throw()
: m_what(problem)
{
    /* empty */
}
//-----------------------------------------------------------------
/**
 * Return info about problem.
 * @return pointer to info which is valid only with this ExInfo
 */
    const char *
ExInfo::what() const throw()
{
    return m_what.c_str();
}
//-----------------------------------------------------------------
/**
 * Add more info.
 * @param name description
 * @param value value
 * @return *this
 */
ExInfo &
ExInfo::addInfo(const std::string &name,
        const std::string &value) throw()
{
    m_what.append("; ");
    m_what.append(name);
    m_what.append("='");
    m_what.append(value);
    m_what.append("'");
    return *this;
}
//-----------------------------------------------------------------
/**
 * Add more info, numeric value.
 * @param name description
 * @param value value
 * @return *this
 */
    ExInfo &
ExInfo::addInfo(const std::string &name, long value) throw()
{
    m_what.append("; ");
    m_what.append(name);
    m_what.append("=");
    m_what.append(StringTool::toString(value));
    return *this;
}

