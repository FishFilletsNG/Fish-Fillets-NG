/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Dialog.h"

#include "Log.h"
#include "ResSoundAgent.h"
#include "SoundAgent.h"
#include "Path.h"

//-----------------------------------------------------------------
/**
 * Create new dialog.
 * Both sound file and subtitles are optional.
 */
Dialog::Dialog(const std::string &lang,
        const std::string &soundfile, const std::string &subtitle)
    : m_soundfile(soundfile), m_lang(lang), m_subtitle(subtitle)
{
    m_sound = NULL;
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
 * Do lazy loading of sound.
 * @return channel number where the sound is played or -1
 */
    int
Dialog::talk()
{
    if (NULL == m_sound && false == m_soundfile.empty()) {
        m_sound = ResSoundAgent::agent()->loadSound(
                Path::dataReadPath(m_soundfile));
    }

    int channel = SoundAgent::agent()->playSound(m_sound);
    if (false == m_subtitle.empty()) {
        runSubtitle();
    }

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
//-----------------------------------------------------------------
/**
 * Minimal time according subtitle length.
 * @return minimal number of cycles for talk
 */
int
Dialog::getMinTime() const
{
    return m_subtitle.size();
}


