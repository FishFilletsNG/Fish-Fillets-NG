/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "BaseException.h"

#include <string>

//-----------------------------------------------------------------
/**
 * Mother of all exception.
 *
 * @param exinfo detail info about problem
 */
BaseException::BaseException(const ExInfo &exinfo)
: std::exception(), m_info(exinfo)
{
    /* empty */
}
//-----------------------------------------------------------------
/**
 * Return text info.
 * @return text info, valid with exception live
 */
const char *
BaseException::what() const throw()
{
    return m_info.what();
}
//-----------------------------------------------------------------
/**
 * Return detail info.
 * @return detail info, valid with exception live
 */
const ExInfo &
BaseException::info() const throw()
{
    return m_info;
}
