/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SoundAgent.h"

#include "OptionAgent.h"
#include "StringMsg.h"
#include "UnknownMsgException.h"

//-----------------------------------------------------------------
/**
 * Set sound and music volume.
 */
    void
SoundAgent::own_init()
{
    reinit();

    registerWatcher("volume_sound");
    registerWatcher("volume_music");
}
//-----------------------------------------------------------------
/**
 * Reininitalize the sound system.
 */
    void
SoundAgent::reinit()
{
    OptionAgent *options = OptionAgent::agent();
    options->setDefault("volume_sound", 90);
    options->setDefault("volume_music", 50);
    setSoundVolume(options->getAsInt("volume_sound"));
    setMusicVolume(options->getAsInt("volume_music"));
}
//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - param_changed(volume_sound) ... set sound volume
 * - param_changed(volume_music) ... set music volume
 *
 * @throws UnknownMsgException
 */
    void
SoundAgent::receiveString(const StringMsg *msg)
{
    if (msg->equalsName("param_changed")) {
        std::string param = msg->getValue();
        if ("volume_sound" == param) {
            int volume = OptionAgent::agent()->getAsInt("volume_sound");
            setSoundVolume(volume);
        }
        else if ("volume_music" == param) {
            int volume = OptionAgent::agent()->getAsInt("volume_music");
            setMusicVolume(volume);
        }
    }
    else {
        throw UnknownMsgException(msg);
    }
}



