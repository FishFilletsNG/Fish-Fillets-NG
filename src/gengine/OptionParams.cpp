/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "OptionParams.h"

#include "Log.h"
#include "Environ.h"

//-----------------------------------------------------------------
void
OptionParams::addParam(const std::string &name, eType type,
        const std::string &help)
{
    Param param = {type, help};
    m_params[name] = param;
    int paramSize = name.size() + getType(type).size();
    if (paramSize > m_maxSize) {
        m_maxSize = paramSize;
    }
}
//-----------------------------------------------------------------
std::string
OptionParams::getHelp(const Environ * /*options*/) const
{
    std::string help;
    t_params::const_iterator end = m_params.end();
    for (t_params::const_iterator i = m_params.begin(); i != end; ++i) {
        int paramSize = i->first.size() + getType(i->second.type).size();
        std::string fill = std::string(m_maxSize - paramSize, ' ');
        help += "  " + i->first + "=<" + getType(i->second.type) + ">";
        help += fill + "    " + i->second.help + "\n";
    }
    return help;
}
//-----------------------------------------------------------------
std::string
OptionParams::getConfig(const Environ *options) const
{
    std::string config;
    t_params::const_iterator end = m_params.end();
    for (t_params::const_iterator i = m_params.begin(); i != end; ++i) {
        config += i->first + "='" + options->getParam(i->first) + "'\n";
    }
    return config;
}
//-----------------------------------------------------------------
std::string
OptionParams::getType(eType type) const
{
    switch (type) {
        case TYPE_NUMBER:
            return "number";
        case TYPE_BOOLEAN:
            return "boolean";
        case TYPE_STRING:
            return "string";
        case TYPE_PATH:
            return "path";
        default:
            LOG_WARNING(ExInfo("unknown param type")
                    .addInfo("type", type));
    }
    return "unknown";
}

