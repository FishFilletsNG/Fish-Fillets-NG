/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "BaseAgent.h"

#include "Log.h"
#include "LogicException.h"

//-----------------------------------------------------------------
BaseAgent::BaseAgent()
{
    m_initialized = false;
}
//-----------------------------------------------------------------
    void
BaseAgent::init()
{
    LOG_DEBUG(ExInfo("init").addInfo("name", getName()));
    //NOTE: agent can call oneself in init()
    m_initialized = true;
    own_init();
}
//-----------------------------------------------------------------
/**
 * @throws LogicException when agent is not initialized.
 */
    void
BaseAgent::update()
{
    if (!m_initialized) {
        throw LogicException(ExInfo("agent is not ready")
            .addInfo("name", getName()));
    }

    own_update();
}
//-----------------------------------------------------------------
    void
BaseAgent::shutdown()
{
    LOG_DEBUG(ExInfo("shutdown").addInfo("name", getName()));
    own_shutdown();
    m_initialized = false;
}

