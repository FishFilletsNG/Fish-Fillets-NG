/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Dialog.h"

#include "ResSoundAgent.h"
#include "SoundAgent.h"

//-----------------------------------------------------------------
Dialog::Dialog(const std::string &lang,
        const Path &soundfile, const std::string &subtitle)
    : m_lang(lang), m_subtitle(subtitle)
{
    m_sound = ResSoundAgent::agent()->loadSound(soundfile);
}
//-----------------------------------------------------------------
Dialog::~Dialog()
{
    if (m_sound) {
        Mix_FreeChunk(m_sound);
    }
}
//-----------------------------------------------------------------
/**
 * Run dialog.
 * @return channel number where the sound is played or -1
 */
    int
Dialog::talk() const
{
    //TODO: play default silence when m_sound is NULL
    int channel = SoundAgent::agent()->playSound(m_sound);
    runSubtitle();

    return channel;
}
//-----------------------------------------------------------------
    void
Dialog::runSubtitle() const
{
    //NOTE: override this method to run subtitles
    LOG_INFO(ExInfo("subtitle")
            .addInfo("content", m_subtitle));
}


