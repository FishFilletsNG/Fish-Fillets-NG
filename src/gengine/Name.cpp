/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Name.h"


//NOTE: timerAgent is before inputAgent. This gives player
// the fastest rection on his input (there is no delay between input,
// game and video update).
const char *Name::APP_NAME = "App";
const char *Name::MESSAGER_NAME = "10messager";
const char *Name::SCRIPT_NAME = "15script";
const char *Name::OPTION_NAME = "20option";

const char *Name::VIDEO_NAME = "30video";
const char *Name::SOUND_NAME = "40sound";
const char *Name::TIMER_NAME = "50timer";

const char *Name::INPUT_NAME = "60input";
const char *Name::SUBTITLE_NAME = "70subtitle";

const char *Name::GAME_NAME = "80game";

