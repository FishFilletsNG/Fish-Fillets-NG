/*
 * Copyright (C) 2009 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#ifdef HAVE_SMPEG
#include "MovieState.h"

#include "SDL_Movie.h"
#include "DemoInput.h"

#include "OptionAgent.h"
#include "VideoAgent.h"
#include "SoundAgent.h"

//-----------------------------------------------------------------
MovieState::MovieState(const Path &file)
    : m_file(file)
{
    takeHandler(new DemoInput(this));
}
//-----------------------------------------------------------------
    void
MovieState::own_initState()
{
    SoundAgent::agent()->stopMusic();
    Mix_CloseAudio();
    m_movie = new SDL_Movie();
    m_movie->Load(m_file.getNative().c_str());

    SMPEG_Info info = m_movie->GetInfo();
    OptionAgent *options = OptionAgent::agent();
    options->setParam("screen_width", info.width);
    options->setParam("screen_height", info.height);
    VideoAgent::agent()->initVideoMode();

    m_movie->Play();
    registerDrawable(m_movie);
}
//-----------------------------------------------------------------
    void
MovieState::own_updateState()
{
    if(m_movie->GetStatus() != SMPEG_PLAYING) {
        quitState();
    }
}
//-----------------------------------------------------------------
    void
MovieState::own_cleanState()
{
    delete m_movie;
    SoundAgent::agent()->reinit();
}

#endif
