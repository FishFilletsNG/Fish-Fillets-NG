/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "LevelStatus.h"

#include "ScriptState.h"
#include "ScriptException.h"
#include "def-script.h"

#include <stdio.h>

//-----------------------------------------------------------------
   inline LevelStatus *
getStatus(lua_State *L)
{
    return static_cast<LevelStatus*>(script_getLeader(L));
}
//-----------------------------------------------------------------
/**
 * void status_readMoves(saved_moves)
 */
    int
LevelStatus::script_status_readMoves(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *saved_moves = luaL_checkstring(L, 1);
    getStatus(L)->readMoves(saved_moves);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
    void
LevelStatus::readMoves(const std::string savedMoves)
{
    m_savedMoves = savedMoves;
}


//-----------------------------------------------------------------
/**
 * Return new prepared script.
 */
    ScriptState *
LevelStatus::createScript()
{
    ScriptState *script = new ScriptState();
    script->registerFunc("status_readMoves", script_status_readMoves);
    script->registerLeader(this);
    return script;
}
//-----------------------------------------------------------------
/**
 * Read the best solution.
 * @return saved_moves
 */
std::string
LevelStatus::readSolvedMoves(const std::string codename)
{
    m_savedMoves = "";

    Path oldSolution = Path::dataReadPath("solved/" + codename + ".lua");
    if (oldSolution.exists()) {
        ScriptState *script = createScript();
        try {
            script->doFile(oldSolution);
            script->doString("status_readMoves(saved_moves)");
        }
        catch (ScriptException &e) {
            LOG_WARNING(e.info());
        }
        delete script;
    }

    return m_savedMoves;
}
//-----------------------------------------------------------------
/**
 * Write best solution to the file.
 * Save moves and models state.
 */
    void
LevelStatus::writeSolvedMoves(const std::string codename,
        const std::string moves)
{
    std::string prevMoves = readSolvedMoves(codename);

    if (prevMoves.empty() || moves.size() < prevMoves.size()) {
        Path file = Path::dataWritePath("solved/" + codename + ".lua");
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

