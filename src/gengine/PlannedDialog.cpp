/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "PlannedDialog.h"

#include "Dialog.h"
#include "Actor.h"

//-----------------------------------------------------------------
/**
 * Structure to store planned dialog.
 * @param actor who will talk
 * @param delay when will talk
 * @param dialog what will talk
 * @param busy whether actor will be set to busy during talk
 */
PlannedDialog::PlannedDialog(Actor *actor, int delay, const Dialog *dialog,
        bool busy)
{
    m_actor = actor;
    m_delay = delay;
    m_dialog = dialog;
    m_channel = -1;
    m_busy = busy;
    m_running = false;
}
//-----------------------------------------------------------------
PlannedDialog::~PlannedDialog()
{
    if (m_running && m_busy) {
        m_actor->setBusy(false);
    }
}
//-----------------------------------------------------------------
/**
 * Run talk.
 */
void
PlannedDialog::talk()
{
    m_channel = m_dialog->talk();
    if (m_channel > -1) {
        m_running = true;
        if (m_busy) {
            m_actor->setBusy(true);
        }
    }
}

//-----------------------------------------------------------------
bool
PlannedDialog::equalsActor(const Actor *other) const
{
    return m_actor->equals(other);
}
//-----------------------------------------------------------------
/**
 * Stop talking.
 */
void
PlannedDialog::killTalk()
{
    if (isTalking()) {
        Mix_HaltChannel(m_channel);
    }
}
//-----------------------------------------------------------------
/**
 * Return true when our channel is playing and
 * our chunk is the last one on this channel.
 */
bool
PlannedDialog::isTalking()
{
    //TODO: return true for minimal time according subtitle length
    bool result = false;
    if (m_channel > -1) {
        if (Mix_Playing(m_channel)) {
            result = m_dialog->equalSound(Mix_GetChunk(m_channel));
        }
    }
    return result;
}
