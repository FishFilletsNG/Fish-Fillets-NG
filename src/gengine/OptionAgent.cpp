/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "OptionAgent.h"

#include "Environ.h"

#include "Log.h"
#include "Path.h"
#include "FsPath.h"
#include "ScriptAgent.h"
#include "StringTool.h"
#include "LogicException.h"
#include "ScriptException.h"
#include "minmax.h"

#include <string.h>
#include <locale.h>

#ifndef LC_MESSAGES
#define LC_MESSAGES LC_CTYPE
#endif

//TODO: set system datadir
#ifndef SYSTEM_DATA_DIR
#define SYSTEM_DATA_DIR ""
#endif

//NOTE: userdir = $HOME + USER_DATA_DIR
#ifndef USER_DATA_DIR
#define USER_DATA_DIR ".fillets-ng"
#endif

const char *OptionAgent::CONFIG_FILE = "script/options.lua";
//-----------------------------------------------------------------
/**
 * Set user and sytem dir
 * and process "script/options.lua" - this will set user and system paths
 * and process "script/init.lua".
 */
    void
OptionAgent::own_init()
{
    m_environ = new Environ();
    prepareDataPaths();
    prepareLang();

    try {
        Path systemConfig = Path::dataSystemPath(CONFIG_FILE);
        if (systemConfig.exists()) {
            ScriptAgent::agent()->doFile(systemConfig);
        }
    }
    catch (ScriptException &e) {
        LOG_WARNING(e.info());
    }
    try {
        Path userConfig = Path::dataUserPath(CONFIG_FILE);
        if (userConfig.exists()) {
            ScriptAgent::agent()->doFile(userConfig);
        }
    }
    catch (ScriptException &e) {
        LOG_WARNING(e.info());
    }
}
//-----------------------------------------------------------------
/**
 * Save user config.
 * Delete left messages.
 */
    void
OptionAgent::own_shutdown()
{
    delete m_environ;
}
//-----------------------------------------------------------------
/**
 * Set user and sytem dir options.
 */
    void
OptionAgent::prepareDataPaths()
{
    OptionAgent::agent()->setParam("systemdir", SYSTEM_DATA_DIR);

    std::string userdir = "";
    const char *home = getenv("HOME");
    if (home) {
        userdir = FsPath::join(home, USER_DATA_DIR);
    }
    OptionAgent::agent()->setParam("userdir", userdir);
}
//-----------------------------------------------------------------
/**
 * Prepare user lang option.
 * For 2-letter lang codes
 * see http://www.w3.org/WAI/ER/IG/ert/iso639.htm
 */
    void
OptionAgent::prepareLang()
{
    setlocale(LC_ALL, "");
    //NOTE: '.' will be decimal point for Lua
    setlocale(LC_NUMERIC, "C");
    if (getParam("lang").empty()) {
        char *form = setlocale(LC_MESSAGES, NULL);
        int size = min(5, strlen(form));
        if (form && size >= 2) {
            setParam("lang", std::string(form, size));
        }
    }
}
//-----------------------------------------------------------------
/**
 * Parse command line options.
 * Format: $0 [name=value ...]
 *
 * @throws LogicException when format is wrong
 */
    void
OptionAgent::parseCmdOpt(int argc, char *argv[])
{
    if (argc >= 1) {
        setParam("program", argv[0]);
    }

    for (int i = 1; i < argc; ++i) {
        std::string name;
        std::string value;
        if (splitOpt(argv[i], &name, &value)) {
            setParam(name, value);
        }
        else {
            throw LogicException(ExInfo("wrong options format")
                    .addInfo("wrong", argv[i])
                    .addInfo("usage", "$0 [name=value ...]"));
        }
    }
}
//-----------------------------------------------------------------
/**
 * Split "name=value".
 * @return true for success
 */
    bool
OptionAgent::splitOpt(const std::string &option,
        std::string *out_name, std::string *out_value)
{
    bool result = false;
    std::string::size_type pos = option.find('=');
    if (pos != std::string::npos) {
        if (pos + 1  < option.size()) {
            *out_name = option.substr(0, pos);
            *out_value = option.substr(pos + 1, std::string::npos);
            result = true;
        }
    }

    return result;
}
//-----------------------------------------------------------------
/**
 * Set param.
 * Notice watchers.
 * When watcher is not available, it will be removed.
 *
 * @param name param name
 * @param value param value
 */
    void
OptionAgent::setParam(const std::string &name, const std::string &value)
{
    m_environ->setParam(name, value);
}
//-----------------------------------------------------------------
/**
 * Store this integer value like string param.
 */
    void
OptionAgent::setParam(const std::string &name, long value)
{
    m_environ->setParam(name, value);
}
//-----------------------------------------------------------------
/**
 * Return value.
 * Implicit value is "".
 *
 * @param name param name
 * @param implicit default value = ""
 * @return value or implicit value
 */
    std::string
OptionAgent::getParam(const std::string &name,
        const std::string &implicit)
{
    return m_environ->getParam(name, implicit);
}
//-----------------------------------------------------------------
/**
 * Return number value.
 * Implicit value is zero.
 *
 * @param name param name
 * @param implicit default value = 0
 * @return number or implicit
 */
    int
OptionAgent::getAsInt(const std::string &name,
        int implicit)
{
    return m_environ->getAsInt(name, implicit);
}
//-----------------------------------------------------------------
/**
 * Set param also on disk.
 * Preserve all current params in options file.
 */
    void
OptionAgent::setPersistent(const std::string &name, const std::string &value)
{
    //NOTE: path must be created before change of environ
    Path config = Path::dataWritePath(CONFIG_FILE);

    Environ *swap_env = m_environ;
    m_environ = new Environ();

    try {
        if (config.exists()) {
            ScriptAgent::agent()->doFile(config);
        }
    }
    catch (ScriptException &e) {
        LOG_WARNING(e.info());
    }
    setParam(name, value);
    m_environ->store(config);

    delete m_environ;
    m_environ = swap_env;
    setParam(name, value);
}
//-----------------------------------------------------------------
    void
OptionAgent::setPersistent(const std::string &name, long value)
{
    setPersistent(name, StringTool::toString(value));
}
//-----------------------------------------------------------------
/**
 * Set value when it is empty.
 */
void
OptionAgent::setDefault(const std::string &name, const std::string &value)
{
    m_environ->setParam(name, m_environ->getParam(name, value));
}
//-----------------------------------------------------------------
void
OptionAgent::setDefault(const std::string &name, int value)
{
    m_environ->setParam(name, m_environ->getAsInt(name, value));
}
//-----------------------------------------------------------------
    void
OptionAgent::addWatcher(const std::string &name, BaseMsg *msg)
{
    m_environ->addWatcher(name, msg);
}

