/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "FsPath.h"

#include "Log.h"

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/convenience.hpp"
#include "boost/filesystem/exception.hpp"
#include "boost/version.hpp"

//-----------------------------------------------------------------
inline boost::filesystem::path
boostPath(const std::string &file)
{
#if BOOST_VERSION < 103100
    return boost::filesystem::path(file);
#else
    return boost::filesystem::path(file,
            boost::filesystem::portable_posix_name);
#endif
}

//-----------------------------------------------------------------
/**
 * Get native filename.
 * @param file posix filename
 */
    std::string
FsPath::getNative(const std::string &file)
{
    return boostPath(file).native_file_string();
}
//-----------------------------------------------------------------
/**
 * Returns true when file or directory exists.
 * @param file posix filename
 */
    bool
FsPath::exists(const std::string &file)
{
    return boost::filesystem::exists(boostPath(file));
}
//-----------------------------------------------------------------
/**
 * Joint two paths.
 * @param dir posix filename
 * @param file posix filename
 * @return "dir/file"
 */
    std::string
FsPath::join(const std::string &dir, const std::string &file)
{
    return (boostPath(dir) / boostPath(file)).string();
}
//-----------------------------------------------------------------
/**
 * Create all directories in path (like "mkdir -p").
 * @param file posix filename
 */
void
FsPath::createPath(const std::string &file)
{
    boost::filesystem::create_directories(boostPath(file).branch_path());
}

