/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "BaseListener.h"

#include "UnknownMsgException.h"
#include "SimpleMsg.h"
#include "IntMsg.h"
#include "StringMsg.h"

//-----------------------------------------------------------------
/**
 * @throws UnknownMsgException
 */
void
BaseListener::receiveSimple(const SimpleMsg *msg)
{
    throw UnknownMsgException(msg);
}
//-----------------------------------------------------------------
/**
 * @throws UnknownMsgException
 */
void
BaseListener::receiveInt(const IntMsg *msg)
{
    throw UnknownMsgException(msg);
}
//-----------------------------------------------------------------
/**
 * @throws UnknownMsgException
 */
void
BaseListener::receiveString(const StringMsg *msg)
{
    throw UnknownMsgException(msg);
}


