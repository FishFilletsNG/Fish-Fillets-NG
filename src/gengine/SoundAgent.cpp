/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SoundAgent.h"

#include "UnknownMsgException.h"

//-----------------------------------------------------------------
/**
 * Inc and Dec volume.
 * Messages:
 * - inc_volume(num) ... inc volume by num (max 100)
 * - dec_volume(num) ... dec volume by num (min 0)
 * @throws UnknownMsgException
 */
void
SoundAgent::receiveInt(const IntMsg *msg)
{
    if ("inc_volume" == msg->getName()) {
        setSoundVolume(getSoundVolume() + msg->getValue());
        setMusicVolume(getMusicVolume() + msg->getValue());
    }
    else if ("dec_volume" == msg->getName()) {
        int volume = getSoundVolume() - msg->getValue();
        setSoundVolume(volume > 0 ? volume : 0);
        volume = getMusicVolume() - msg->getValue();
        setMusicVolume(volume > 0 ? volume : 0);
    }
    else {
        throw UnknownMsgException(msg);
    }
}
