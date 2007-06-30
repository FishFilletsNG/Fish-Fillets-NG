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
#include "SoundAgent.h"
#include "ResSoundPack.h"
#include "Path.h"
#include "minmax.h"

const std::string Dialog::DEFAULT_LANG = "en";
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
 *
 * @param volume sound volume
 * @param loops numer of loops. 0=play once, 1=play twice, -1=play infinite
 * @return channel number where the sound is played or -1
 */
    int
Dialog::talk(int volume, int loops) const
{
    if (NULL == m_sound && !m_soundfile.empty()) {
        Path soundPath = Path::dataReadPath(m_soundfile);
        m_sound = ResSoundPack::loadSound(soundPath);
    }

    int channel = SoundAgent::agent()->playSound(m_sound, volume, loops);
    return channel;
}
//-----------------------------------------------------------------
/**
 * Override this method to run subtitles.
 */
    void
Dialog::runSubtitle(const StringTool::t_args &args) const
{
    LOG_INFO(ExInfo("subtitle")
            .addInfo("content", getFormatedSubtitle(args)));
}
//-----------------------------------------------------------------
/**
 * Replace %1, %2, ... with the arguments.
 * NOTE: %0 is not expanded
 */
std::string
Dialog::getFormatedSubtitle(const StringTool::t_args &args) const
{
    std::string buffer = m_subtitle;
    for (unsigned int i = 1; i < args.size(); ++i) {
        StringTool::replace(buffer,
                "%" + StringTool::toString(i), args[i]);
    }
    return buffer;
}
//-----------------------------------------------------------------
/**
 * Minimal time according subtitle length.
 * @return minimal number of cycles for talk
 */
int
Dialog::getMinTime() const
{
    return min(180, StringTool::utf8Length(m_subtitle));
}


