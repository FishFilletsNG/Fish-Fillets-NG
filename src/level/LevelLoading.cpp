/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LevelLoading.h"

#include "RoomAccess.h"
#include "Room.h"
#include "LoadException.h"
#include "minmax.h"

//-----------------------------------------------------------------
LevelLoading::LevelLoading(RoomAccess *access)
{
    m_access = access;
    reset();
}
//-----------------------------------------------------------------
    void
LevelLoading::reset()
{
    m_paused = false;
    m_replayMode = false;
    m_loadSpeed = 1;
    m_loadedMoves = "";
}
//-----------------------------------------------------------------
bool
LevelLoading::isLoading() const
{
    return !m_loadedMoves.empty() || m_replayMode;
}
//-----------------------------------------------------------------
/**
 * Start loading mode.
 * @param moves saved moves to load
 */
    void
LevelLoading::loadGame(const std::string &moves)
{
    m_loadedMoves = moves;
    m_loadSpeed = min(50, max(5, m_loadedMoves.size() / 150));
}
//-----------------------------------------------------------------
/**
 * Start replay mode.
 * @param moves saved moves to load
 */
    void
LevelLoading::loadReplay(const std::string &moves)
{
    m_loadedMoves = moves;
    m_loadSpeed = 1;
    m_replayMode = true;
}
//-----------------------------------------------------------------
/**
 * Load a few moves.
 * @throws LoadException for bad load
 */
    void
LevelLoading::nextLoadAction()
{
    if (m_paused) {
        return;
    }

    if (m_loadedMoves.empty()) {
        m_access->room()->beginFall(false);
        m_access->room()->finishRound(false);
    }
    else {
        for (int i = 0; i < m_loadSpeed
                && !m_loadedMoves.empty(); ++i)
        {
            try {
                char symbol = m_loadedMoves[0];
                m_loadedMoves.erase(0, 1);

                m_access->room()->loadMove(symbol);
            }
            catch (LoadException &e) {
                throw LoadException(ExInfo(e.info())
                        .addInfo("remain", m_loadedMoves));
            }
        }
    }
}

