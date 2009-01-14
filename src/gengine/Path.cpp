/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Path.h"

#include "Log.h"
#include "OptionAgent.h"
#include "FsPath.h"
#include "PathException.h"

#include <stdio.h>

//-----------------------------------------------------------------
    Path::Path(const std::string &file)
: m_path(file)
{
    /* empty */
}
//-----------------------------------------------------------------
/**
 * Try return user data path,
 * otherwise return system data path.
 * NOTE: OptionAgent must be initialized before this
 *
 * @param file path to file
 * @param writeable whether we want write to the file
 * @return path to user or system file
 */
    Path
Path::dataPath(const std::string &file, bool writeable)
{
    Path datapath = dataUserPath(file);

    if (!datapath.exists())  {
        FILE *try_open = NULL;
        if (writeable) {
            try {
                LOG_INFO(ExInfo("creating path")
                        .addInfo("path", datapath.getNative()));
                FsPath::createPath(datapath.getPosixName());

                try_open = fopen(datapath.getNative().c_str(), "wb");
            }
            catch (PathException &e) {
                LOG_WARNING(e.info());
            }
        }

        if (try_open) {
            fclose(try_open);
        }
        else {
            datapath = dataSystemPath(file);
        }
    }

    return datapath;
}
//-----------------------------------------------------------------
    Path
Path::dataReadPath(const std::string &file)
{
    return dataPath(file, false);
}
//-----------------------------------------------------------------
    Path
Path::dataWritePath(const std::string &file)
{
    return dataPath(file, true);
}
//-----------------------------------------------------------------
/**
 * Return path to system file.
 * Path does not need to exist.
 */
    Path
Path::dataSystemPath(const std::string &file)
{
    std::string systemdir = OptionAgent::agent()->getParam("systemdir");
    return Path(FsPath::join(systemdir, file));
}
//-----------------------------------------------------------------
/**
 * Return path to user file.
 * Path does not need to exist.
 */
    Path
Path::dataUserPath(const std::string &file)
{
    std::string userdir = OptionAgent::agent()->getParam("userdir");
    return Path(FsPath::join(userdir, file));
}

//-----------------------------------------------------------------
std::string
Path::getNative() const
{
    return FsPath::getNative(m_path);
}
//-----------------------------------------------------------------
bool
Path::exists() const
{
    return FsPath::exists(m_path);
}
