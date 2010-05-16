/*
 * Copyright (C) 2010 Brian Raiter
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SDLMusicLooper.h"

#include "Path.h"
#include "StringTool.h"
#include "Log.h"

#include <string.h> //memset

//-----------------------------------------------------------------
/**
 * Initialize the player for a specific piece of music.
 */
    SDLMusicLooper::SDLMusicLooper(const Path &file)
: m_volume(MIX_MAX_VOLUME), m_position(0)
{
    Uint16 fmt;
    int freq, channels;
    Mix_QuerySpec(&freq, &fmt, &channels);
    int sampleSize = fmt == AUDIO_S8 || fmt == AUDIO_U8 ? 1 : 2;

    m_music = Mix_LoadWAV(file.getNative().c_str());
    if (m_music) {
        lookupLoopData(file, sampleSize * channels * freq / 22050);
    }
    else {
        LOG_WARNING(ExInfo("cannot play music")
                .addInfo("music", file.getNative())
                .addInfo("Mix", Mix_GetError()));
    }
}
//-----------------------------------------------------------------
SDLMusicLooper::~SDLMusicLooper()
{
    stop();
    if (m_music) {
        Mix_FreeChunk(m_music);
    }
}
//-----------------------------------------------------------------
void
SDLMusicLooper::setVolume(int volume)
{
    m_volume = volume;
    if (m_volume > MIX_MAX_VOLUME) {
        m_volume = MIX_MAX_VOLUME;
    }
    else if (m_volume < 0) {
        m_volume = 0;
    }
}
//-----------------------------------------------------------------
void
SDLMusicLooper::start()
{
    if (m_music) {
        Mix_HookMusic(musicOutput, (void*)this);
    }
}
//-----------------------------------------------------------------
void
SDLMusicLooper::stop()
{
    Mix_HookMusic(NULL, NULL);
}
//-----------------------------------------------------------------
/**
 * Check for a loop associated with this filename. If not, then
 * loop over the entire chunk.
 *
 * In the original game, the music loop definitions were stored as metadata
 * inside the sampler chunk of the WAV files.
 * Now we store them in *.ogg.meta.
 *
 * File format:
 * loop_start
 * loop_end
 */
void
SDLMusicLooper::lookupLoopData(const Path &file, int multiplier)
{
    m_startLoop = 0;
    m_endLoop = m_music->alen;

    std::string metafile = file.getNative() + ".meta";
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    FILE *meta = fopen(metafile.c_str(), "r");
    if(!meta) {
        return;
    }

    if (!fread(&buffer, 1, sizeof(buffer) - 1, meta)) {
        LOG_WARNING(ExInfo("unable to read music meta data")
                .addInfo("file", metafile));
        return;
    }

    StringTool::t_args lines = StringTool::split(buffer, '\n');
    if (lines.size() < 2) {
        LOG_WARNING(ExInfo("unrecognized music meta data format")
                .addInfo("file", metafile));
        return;
    }

    m_startLoop = strtol(lines[0].c_str(), NULL, 10) * multiplier;
    m_endLoop = strtol(lines[1].c_str(), NULL, 10) * multiplier;
    if ((unsigned int)m_endLoop > m_music->alen) {
        m_endLoop = m_music->alen;
    }
    LOG_DEBUG(ExInfo("looping music")
            .addInfo("start", m_startLoop / multiplier)
            .addInfo("end", m_endLoop / multiplier));
}
//-----------------------------------------------------------------
/**
 * Callback function that provides the music data to the mixer.
 */
void
SDLMusicLooper::musicOutput(void *udata, Uint8 *stream, int length)
{
    SDLMusicLooper *that = (SDLMusicLooper*)udata;
    int n;

    memset(stream, 0, length);
    while (length >= that->m_endLoop - that->m_position) {
        n = that->m_endLoop - that->m_position;
        SDL_MixAudio(stream, that->m_music->abuf + that->m_position, n,
                     that->m_volume);
        stream += n;
        length -= n;
        that->m_position = that->m_startLoop;
    }
    SDL_MixAudio(stream, that->m_music->abuf + that->m_position, length,
                 that->m_volume);
    that->m_position += length;
}
