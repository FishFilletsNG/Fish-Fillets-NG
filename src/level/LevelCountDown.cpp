/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LevelCountDown.h"

#include "LevelStatus.h"
#include "RoomAccess.h"
#include "Room.h"
#include "StepCounter.h"
#include "LogicException.h"
#include "CountAdvisor.h"

//-----------------------------------------------------------------
/**
 * Prepare countdown.
 */
LevelCountDown::LevelCountDown(const RoomAccess *access)
{
    m_countdown = -1;
    m_access = access;
    m_levelStatus = NULL;
}
//-----------------------------------------------------------------
/**
 * Resets counter.
 * @throws LogicException when levelStatus is not filled
 */
void
LevelCountDown::reset()
{
    if (NULL == m_levelStatus) {
        throw LogicException(ExInfo("level status is NULL"));
    }
    m_levelStatus->setRunning(true);
    m_countdown = -1;
}
//-----------------------------------------------------------------
/**
 * Countdown to zero.
 * @param advisor advisor which know usable coundown values
 * @return true when counter is at zero
 */
bool
LevelCountDown::countDown(const CountAdvisor *advisor)
{
    bool result = false;
    if (m_countdown < 0) {
        setCountDown(advisor);
    }
    else if (m_countdown > 0) {
        m_countdown--;
    }
    else {
        result = true;
    }
    return result;
}
//-----------------------------------------------------------------
void
LevelCountDown::setCountDown(const CountAdvisor *advisor)
{
    if (m_access->const_room()->isSolved()) {
        m_countdown = advisor->getCountForSolved();
    }
    else if (m_access->const_room()->cannotMove()) {
        m_countdown = advisor->getCountForWrong();
    }
    else {
        m_countdown = -1;
    }
}
//-----------------------------------------------------------------
bool
LevelCountDown::isFinishedEnough() const
{
    return m_countdown == 0 && m_access->const_room()->isSolved();
}
//-----------------------------------------------------------------
bool
LevelCountDown::isWrongEnough() const
{
    return m_countdown == 0 && m_access->const_room()->cannotMove() &&
        !m_access->const_room()->isSolved();
}
//-----------------------------------------------------------------
/**
 * Write best solution to the file.
 * Save moves and models state.
 */
    void
LevelCountDown::saveSolution()
{
    m_levelStatus->setComplete();
    std::string current_moves =
        m_access->const_room()->stepCounter()->getMoves();
    m_levelStatus->writeSolvedMoves(current_moves);
}
//-----------------------------------------------------------------
/**
 * Creates next state or returns NULL.
 * @return returns NULL when only quitState() is needed
 */
GameState *
LevelCountDown::createNextState()
{
    return m_levelStatus->createPoster();
}

