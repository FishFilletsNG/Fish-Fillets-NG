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
#include "OptionAgent.h"
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

    if (false == m_planned.empty()) {
        if (m_running.empty()) {
            PlannedDialog *dialog = m_planned.front();
            if (dialog->getDelay() > 0) {
                dialog->decDelay();
            }
            else {
                dialog->talk();
                m_planned.pop_front();
                m_running.push_back(dialog);
            }
        }
    }
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
 * Run this dialog after delay cycles.
 * @param name dialog name
 * @param delay delay in cycles after last dialog
 * @param actor who is talking
 * @param busy whether actor will be busy during the talk
 */
    void
DialogAgent::planDialog(const std::string &name, int delay, Actor *actor,
        bool busy)
{
    std::string lang = OptionAgent::agent()->getParam("lang", "en");
    Dialog *dialog = m_dialogs->findDialog(name, lang);
    if (dialog) {
        PlannedDialog *plan = new PlannedDialog(actor, delay, dialog, busy);
        m_planned.push_back(plan);

        LOG_DEBUG(ExInfo("planned dialog")
                .addInfo("name", name)
                .addInfo("delay", delay));
    }
}
//-----------------------------------------------------------------
    bool
DialogAgent::isTalking(const Actor *actor)
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
        if (false == (*i)->isTalking()) {
            delete *i;
            m_running.erase(i);
            return;
        }
    }
}
//-----------------------------------------------------------------
/**
 * Delete all planned and running dialogs from this actor.
 */
    void
DialogAgent::killSound(const Actor *actor)
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

    while (killOnePlanned(actor)) {
        /* empty */
    }
}
//-----------------------------------------------------------------
/**
 * Kill one planned dialog with this actor.
 * @return true when some dialog was killed
 */
    bool
DialogAgent::killOnePlanned(const Actor *actor)
{
    //NOTE: erase on deque invalidates all iterators
    t_planned::iterator end = m_planned.end();
    for (t_planned::iterator i = m_planned.begin(); i != end; ++i) {
        if ((*i)->equalsActor(actor)) {
            delete *i;
            m_planned.erase(i);
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------
/**
 * Delete all dialogs, including planned dialogs.
 */
    void
DialogAgent::removeAll()
{
    t_planned::iterator plan_end = m_planned.end();
    for (t_planned::iterator i = m_planned.begin(); i != plan_end; ++i) {
        delete *i;
    }
    m_planned.clear();

    t_running::iterator run_end = m_running.end();
    for (t_running::iterator j = m_running.begin(); j != run_end; ++j) {
        (*j)->killTalk();
        delete *j;
    }
    m_running.clear();

    m_dialogs->removeAll();
}
//-----------------------------------------------------------------
/**
 * Returns true when there is no planned or running dialog.
 */
bool
DialogAgent::empty()
{
    return m_planned.empty() && m_running.empty();
}

