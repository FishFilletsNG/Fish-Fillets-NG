/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SelectLang.h"

#include "HBox.h"
#include "RadioBox.h"

#include "def-script.h"
#include "ScriptState.h"

//-----------------------------------------------------------------
    inline SelectLang *
getSelect(lua_State *L)
{
    return static_cast<SelectLang*>(script_getLeader(L));
}
//-----------------------------------------------------------------
/**
 * void select_addFlag(lang, picture)
 */
    int
script_select_addFlag(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *lang = luaL_checkstring(L, 1);
    const char *picture = luaL_checkstring(L, 2);

    getSelect(L)->addFlag(lang, Path::dataReadPath(picture));
    END_NOEXCEPTION;
    return 0;
}

//-----------------------------------------------------------------
/**
 * Execute script which will add flags.
 */
SelectLang::SelectLang(const Path &datafile)
{
    m_activeRow = new HBox();

    ScriptState script;
    script.registerLeader(this);
    script.registerFunc("select_addFlag", script_select_addFlag);
    script.doFile(datafile);

    addWidget(m_activeRow);
}
//-----------------------------------------------------------------
/**
 * Stack flags in table.
 */
    void
SelectLang::addFlag(const std::string &lang, const Path &picture)
{
    m_activeRow->addWidget(new RadioBox("lang", lang, picture));
    if (m_activeRow->getW() > MAX_WIDTH) {
        addWidget(m_activeRow);
        m_activeRow = new HBox();
    }
}

