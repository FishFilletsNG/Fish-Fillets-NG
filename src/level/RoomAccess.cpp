/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "RoomAccess.h"

#include "Room.h"
#include "LogicException.h"

//-----------------------------------------------------------------
/**
 * Room is not set.
 */
RoomAccess::RoomAccess()
{
    m_room = NULL;
}
//-----------------------------------------------------------------
RoomAccess::~RoomAccess()
{
    cleanRoom();
}
//-----------------------------------------------------------------
    void
RoomAccess::takeRoom(Room *new_room)
{
    cleanRoom();
    m_room = new_room;
}
//-----------------------------------------------------------------
/**
 * Remove old room.
 */
    void
RoomAccess::cleanRoom()
{
    if (m_room) {
        delete m_room;
        m_room = NULL;
    }
}
//-----------------------------------------------------------------
/**
 * Check whether room is ready.
 * @throws LogicException when room is not ready
 */
void
RoomAccess::checkRoom() const
{
    if (NULL == m_room) {
        throw LogicException(ExInfo("room is not ready"));
    }
}
//-----------------------------------------------------------------
/**
 * Returns room or throws exception.
 */
    Room *
RoomAccess::room()
{
    checkRoom();
    return m_room;
}
//-----------------------------------------------------------------
/**
 * Returns room or throws exception.
 */
    const Room *
RoomAccess::const_room() const
{
    checkRoom();
    return m_room;
}

