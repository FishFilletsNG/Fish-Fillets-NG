/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Log.h"

#include "ExInfo.h"

#include <stdlib.h>
#include <stdio.h>

int Log::ms_logLevel = LEVEL_INFO;
//int Log::ms_logLevel = LEVEL_DEBUG;
//-----------------------------------------------------------------
/**
 * Print log info.
 * @param level priority level
 * @param file source file
 * @param line line number in source file
 * @param info detail info
 */
void 
Log::log(int level, const char *file, int line,
        const ExInfo &info) throw()
{
    if (level <= ms_logLevel) {
        const char *levelName;
        switch (level) {
            case LEVEL_DEBUG:
                levelName = "DEBUG";
                break;
            case LEVEL_INFO:
                levelName = "INFO";
                break;
            case LEVEL_WARNING:
                levelName = "WARNING";
                break;
            case LEVEL_ERROR:
                levelName = "ERROR";
                break;
            default:
                levelName = "UNKNOWN";
                break;
        }

        fprintf(stderr, "%s:%d: %s %s\n", file, line, levelName, info.what());
    }
}

