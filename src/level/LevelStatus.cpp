/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LevelStatus.h"

#include "def-script.h"
#include "Path.h"
#include "ScriptState.h"
#include "ScriptException.h"
#include "DemoMode.h"

extern "C" {
#include "lua.h"
}
#include <stdio.h>

//-----------------------------------------------------------------
   inline LevelStatus *
getStatus(lua_State *L)
{
    return dynamic_cast<LevelStatus*>(script_getLeader(L));
}


//-----------------------------------------------------------------
/**
 * void status_readMoves(saved_moves)
 */
    static int
script_status_readMoves(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *saved_moves = luaL_checkstring(L, 1);
    getStatus(L)->readMoves(saved_moves);
    END_NOEXCEPTION;
    return 0;
}

//-----------------------------------------------------------------
LevelStatus::LevelStatus()
{
    m_bestMoves = -1;
    m_complete = false;
    m_wasRunning = false;
    m_script->registerFunc("status_readMoves", script_status_readMoves);
}
//-----------------------------------------------------------------
    void
LevelStatus::readMoves(const std::string &savedMoves)
{
    m_savedMoves = savedMoves;
}
//-----------------------------------------------------------------
void
LevelStatus::prepareRun(const std::string &codename, const std::string &poster,
        int bestMoves, const std::string &bestAuthor)
{
    m_complete = false;
    m_wasRunning = false;
    m_codename = codename;
    m_poster = poster;
    m_bestMoves = bestMoves;
    m_bestAuthor = bestAuthor;
}
//-----------------------------------------------------------------
std::string
LevelStatus::getSolutionFilename(const std::string &codename)
{
    return "solved/" + codename + ".lua";
}
//-----------------------------------------------------------------
std::string
LevelStatus::getSolutionFilename() const
{
    return getSolutionFilename(m_codename);
}
//-----------------------------------------------------------------
/**
 * Read the best solution.
 * @return saved_moves or empty string
 */
std::string
LevelStatus::readSolvedMoves()
{
    m_savedMoves = "";

    Path oldSolution = Path::dataReadPath(getSolutionFilename());
    if (oldSolution.exists()) {
        try {
            scriptDo("saved_moves=nil");
            scriptInclude(oldSolution);
            scriptDo("status_readMoves(saved_moves)");
        }
        catch (ScriptException &e) {
            LOG_WARNING(e.info());
        }
    }

    return m_savedMoves;
}
//-----------------------------------------------------------------
/**
 * Write best solution to the file.
 * Save moves and models state.
 */
    void
LevelStatus::writeSolvedMoves(const std::string &moves)
{
    std::string prevMoves = readSolvedMoves();

    if (prevMoves.empty() || moves.size() < prevMoves.size()) {
        Path file = Path::dataWritePath(getSolutionFilename());
        FILE *saveFile = fopen(file.getNative().c_str(), "w");
        if (saveFile) {
            fputs("\nsaved_moves = '", saveFile);
            fputs(moves.c_str(), saveFile);
            fputs("'\n", saveFile);
            fclose(saveFile);
        }
        else {
            LOG_WARNING(ExInfo("cannot save solution")
                    .addInfo("file", file.getNative())
                    .addInfo("moves", moves));
        }
    }
}
//-----------------------------------------------------------------
/**
 * Returns DemoMode or NULL.
 */
GameState *
LevelStatus::createPoster() const
{
    DemoMode *result = NULL;
    if (!m_poster.empty()) {
        result = new DemoMode(Path::dataReadPath(m_poster));
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Compares this player and the best one.
 * @returns -1 (this is worse), 0 (equals) or 1 (the best)
 */
int
LevelStatus::compareToBest()
{
    int moves = readSolvedMoves().size();
    int result = 1;
    if (m_bestMoves > 0) {
        if (m_bestMoves < moves) {
            result = -1;
        }
        else if (m_bestMoves == moves) {
            result = 0;
        }
    }
    return result;
}

