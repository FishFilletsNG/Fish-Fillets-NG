/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "GameInput.h"

#include "Log.h"
#include "KeyConsole.h"
#include "MenuOptions.h"
#include "OptionAgent.h"

//-----------------------------------------------------------------
/**
 * Push menu state at top.
 */
void
GameInput::enableMenu()
{
    m_state->pushState(new MenuOptions());
}
//-----------------------------------------------------------------
    void
GameInput::toggleSubtitles()
{
    OptionAgent *option = OptionAgent::agent();
    bool enable = option->getAsInt("subtitles");
    if (!enable) {
        LOG_INFO(ExInfo("enabled subtitles"));
    }
    else {
        LOG_INFO(ExInfo("disabled subtitles"));
    }

    option->setPersistent("subtitles", !enable);
}

