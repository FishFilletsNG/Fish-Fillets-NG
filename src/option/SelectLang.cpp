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
#include "Path.h"
#include "ScriptState.h"

//-----------------------------------------------------------------
    inline SelectLang *
getSelect(lua_State *L)
{
    return dynamic_cast<SelectLang*>(script_getLeader(L));
}
//-----------------------------------------------------------------
/**
 * void select_addFlag(value, picture)
 */
    int
script_select_addFlag(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *value = luaL_checkstring(L, 1);
    const char *picture = luaL_checkstring(L, 2);

    getSelect(L)->addFlag(value, Path::dataReadPath(picture));
    END_NOEXCEPTION;
    return 0;
}

//-----------------------------------------------------------------
/**
 * Execute script which will add flags.
 */
SelectLang::SelectLang(const std::string &option, const Path &datafile)
{
    m_option = option;
    m_activeRow = new HBox();

    m_script->registerFunc("select_addFlag", script_select_addFlag);
    scriptInclude(datafile);

    addWidget(m_activeRow);
}
//-----------------------------------------------------------------
/**
 * Stack flags in table.
 */
    void
SelectLang::addFlag(const std::string &value, const Path &picture)
{
    IWidget *flag = new RadioBox(m_option, value, picture);
    flag->setTip(value);
    m_activeRow->addWidget(flag);
    if (m_activeRow->getW() > MAX_WIDTH) {
        addWidget(m_activeRow);
        m_activeRow = new HBox();
    }
}

