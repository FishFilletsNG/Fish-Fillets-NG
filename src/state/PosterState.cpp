/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "PosterState.h"

#include "Path.h"
#include "Picture.h"
#include "DemoInput.h"

#include "OptionAgent.h"
#include "VideoAgent.h"

//-----------------------------------------------------------------
PosterState::PosterState(const Path &picture)
{
    m_bg = new Picture(picture, V2(0, 0));
    takeHandler(new DemoInput(this));
    registerDrawable(m_bg);
}
//-----------------------------------------------------------------
PosterState::~PosterState()
{
    delete m_bg;
}
//-----------------------------------------------------------------
    void
PosterState::own_initState()
{
    OptionAgent *options = OptionAgent::agent();
    options->setParam("screen_width", m_bg->getW());
    options->setParam("screen_height", m_bg->getH());
    VideoAgent::agent()->initVideoMode();
}
