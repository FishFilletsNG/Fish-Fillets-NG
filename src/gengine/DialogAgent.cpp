/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "DialogAgent.h"

#include "Dialog.h"
#include "ResDialogPack.h"
#include "PlannedDialog.h"

//-----------------------------------------------------------------
    void
DialogAgent::own_init()
{
    m_dialogs = new ResDialogPack();
}
//-----------------------------------------------------------------
/**
 * Run planned dialog.
 * Move first planned dialog into the running dialogs.
 */
    void
DialogAgent::own_update()
{
    removeFirstNotTalking();
}
//-----------------------------------------------------------------
    void
DialogAgent::own_shutdown()
{
    removeAll();
    delete m_dialogs;
}
//-----------------------------------------------------------------
/**
 * Store new dialog.
 * @throws NameException when dialog with this name exists
 */
    void
DialogAgent::addDialog(const std::string &name, Dialog *dialog)
{
    m_dialogs->addRes(name, dialog);
}
//-----------------------------------------------------------------
/**
 * Run talk.
 * TODO: allow to set volume
 * @param actor actor index
 * @param name dialog name
 */
    void
DialogAgent::actorTalk(int actor, const std::string &name)
{
    Dialog *dialog = m_dialogs->findDialogHard(name);
    if (dialog) {
        PlannedDialog *talker = new PlannedDialog(actor, dialog);
        talker->talk();
        m_running.push_back(talker);
    }
}
//-----------------------------------------------------------------
    bool
DialogAgent::isTalking(int actor)
{
    t_running::iterator end = m_running.end();
    for (t_running::iterator i = m_running.begin(); i != end; ++i) {
        if ((*i)->equalsActor(actor) && (*i)->isTalking()) {
            return true;
        }
    }
    return false;
}
//-----------------------------------------------------------------
/**
 * Remove first not talking dialog from m_running.
 */
    void
DialogAgent::removeFirstNotTalking()
{
    t_running::iterator end = m_running.end();
    for (t_running::iterator i = m_running.begin(); i != end; ++i) {
        if (!(*i)->isTalking()) {
            delete *i;
            m_running.erase(i);
            return;
        }
    }
}
//-----------------------------------------------------------------
/**
 * Delete all running dialogs made by this actor.
 */
    void
DialogAgent::killSound(int actor)
{
    //NOTE: erase on list invalidates only the erased iterator
    t_running::iterator run_end = m_running.end();
    for (t_running::iterator i = m_running.begin(); i != run_end; /* empty */) {
        t_running::iterator toKill = i;
        ++i;

        if ((*toKill)->equalsActor(actor)) {
            (*toKill)->killTalk();
            delete *toKill;
            m_running.erase(toKill);
        }
    }
}

//-----------------------------------------------------------------
/**
 * Kill all running dialogs.
 */
    void
DialogAgent::killTalks()
{
    t_running::iterator end = m_running.end();
    for (t_running::iterator i = m_running.begin(); i != end; ++i) {
        (*i)->killTalk();
        delete *i;
    }
    m_running.clear();
}
//-----------------------------------------------------------------
/**
 * Delete all shared dialogs and kill talks.
 */
    void
DialogAgent::removeAll()
{
    killTalks();
    m_dialogs->removeAll();
}
//-----------------------------------------------------------------
/**
 * Returns true when there is no running dialog.
 */
bool
DialogAgent::empty()
{
    return m_running.empty();
}

