/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Labels.h"

#include "Path.h"
#include "ResDialogPack.h"
#include "Dialog.h"
#include "ScriptState.h"
#include "def-script.h"

//-----------------------------------------------------------------
    inline Labels *
getLabels(lua_State *L)
{
    return dynamic_cast<Labels*>(script_getLeader(L));
}
//-----------------------------------------------------------------
/**
 * void label_text(labelName, lang, text)
 */
    int
script_label_text(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *labelName = luaL_checkstring(L, 1);
    const char *lang = luaL_checkstring(L, 2);
    const char *text = luaL_checkstring(L, 3);

    getLabels(L)->addLabel(labelName, lang, text);
    END_NOEXCEPTION;
    return 0;
}

//-----------------------------------------------------------------
Labels::Labels(const Path &source)
{
    m_labels = new ResDialogPack();
    m_script->registerFunc("label_text", script_label_text);
    scriptInclude(source);
}
//-----------------------------------------------------------------
Labels::~Labels()
{
    m_labels->removeAll();
    delete m_labels;
}
//-----------------------------------------------------------------
void
Labels::addLabel(const std::string &name, const std::string &lang,
        const std::string &text)
{
    m_labels->addRes(name, new Dialog(lang, "", text));
}
//-----------------------------------------------------------------
std::string
Labels::getLabel(const std::string &name) const
{
    std::string result;
    const Dialog *dialog = m_labels->findDialogHard(name);
    if (dialog) {
        result = dialog->getSubtitle();
    }
    else {
        result = "???";
    }
    return result;
}
//-----------------------------------------------------------------
std::string
Labels::getFormatedLabel(const std::string &name,
        const StringTool::t_args &args) const
{
    std::string result;
    const Dialog *dialog = m_labels->findDialogHard(name);
    if (dialog) {
        result = dialog->getFormatedSubtitle(args);
    }
    else {
        result = "???";
    }
    return result;
}

