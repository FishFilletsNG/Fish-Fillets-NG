/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "DialogStack.h"

#include "Dialog.h"
#include "ResDialogPack.h"
#include "PlannedDialog.h"
#include "StringTool.h"

//-----------------------------------------------------------------
DialogStack::DialogStack()
{
    m_dialogs = new ResDialogPack();
    m_activeDialog = NULL;
}
//-----------------------------------------------------------------
/**
 * Releases resources and stops all cycling dialogs.
 */
DialogStack::~DialogStack()
{
    removeAll();
    delete m_dialogs;
}
//-----------------------------------------------------------------
/**
 * Removes finished dialogs from stack.
 */
    void
DialogStack::updateStack()
{
    removeFirstNotTalking();
}
//-----------------------------------------------------------------
/**
 * Store new dialog.
 */
    void
DialogStack::addDialog(const std::string &name, Dialog *dialog)
{
    m_dialogs->addRes(name, dialog);
}
//-----------------------------------------------------------------
/**
 * Run talk.
 * Dialog name could contain "name@arg1@arg2..."
 * to fill %1, %2, ... in subtitles.
 *
 * @param actor actor index
 * @param name dialog name
 * @param volume dialog volume
 * @param loops number of loops, 0=play once, 1=play twice, -1=play infinite
 * @param dialogFlag whether it is blocking dialog
 */
    void
DialogStack::actorTalk(int actor, const std::string &name,
        int volume, int loops, bool dialogFlag)
{
    StringTool::t_args args = StringTool::split(name, '@');

    const Dialog *subtitle = m_dialogs->findDialogHard(args[0]);
    if (subtitle) {
        subtitle->runSubtitle(args);

        const Dialog *dialog = m_dialogs->findDialogSpeech(args[0]);
        if (dialog) {
            PlannedDialog *talker = new PlannedDialog(actor, dialog,
                subtitle->getMinTime());
            talker->talk(volume, loops);

            if (loops == -1) {
                m_cycling.push_back(talker);
            }
            else {
                m_running.push_back(talker);
            }

            if (dialogFlag) {
                m_activeDialog = talker;
            }
        }
    }
}
//-----------------------------------------------------------------
    bool
DialogStack::isTalking(int actor) const
{
    return isTalkingIn(actor, m_running) ||
        isTalkingIn(actor, m_cycling);
}
//-----------------------------------------------------------------
    bool
DialogStack::isTalkingIn(int actor, const t_running &fifo) const
{
    t_running::const_iterator end = fifo.end();
    for (t_running::const_iterator i = fifo.begin(); i != end; ++i) {
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
DialogStack::removeFirstNotTalking()
{
    t_running::iterator end = m_running.end();
    for (t_running::iterator i = m_running.begin(); i != end; ++i) {
        if (!(*i)->isTalking()) {
            releaseDialog(*i);
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
DialogStack::killSound(int actor)
{
    killSoundIn(actor, m_running);
    killSoundIn(actor, m_cycling);
}
//-----------------------------------------------------------------
void
DialogStack::killSoundIn(int actor, t_running &fifo)
{
    //NOTE: erase on list invalidates only the erased iterator
    t_running::iterator run_end = fifo.end();
    for (t_running::iterator i = fifo.begin(); i != run_end; /* empty */) {
        t_running::iterator toKill = i;
        ++i;

        if ((*toKill)->equalsActor(actor)) {
            releaseDialog(*toKill);
            fifo.erase(toKill);
        }
    }
}

//-----------------------------------------------------------------
/**
 * Kill all running dialogs.
 */
    void
DialogStack::killTalks()
{
    killTalksIn(m_running);
    killTalksIn(m_cycling);
}
//-----------------------------------------------------------------
void
DialogStack::killTalksIn(t_running &fifo)
{
    t_running::iterator end = fifo.end();
    for (t_running::iterator i = fifo.begin(); i != end; ++i) {
        releaseDialog(*i);
    }
    fifo.clear();
}
//-----------------------------------------------------------------
void
DialogStack::releaseDialog(PlannedDialog *dialog)
{
    if (dialog == m_activeDialog) {
        m_activeDialog = NULL;
    }
    dialog->killTalk();
    delete dialog;
}
//-----------------------------------------------------------------
/**
 * Delete all shared dialogs and kill talks.
 */
    void
DialogStack::removeAll()
{
    killTalks();
    m_dialogs->removeAll();
}
//-----------------------------------------------------------------
/**
 * Returns true when there is active dialog.
 */
bool
DialogStack::isDialog() const
{
    return m_activeDialog && m_activeDialog->isTalking();
}

