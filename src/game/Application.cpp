/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Application.h"

#include "Log.h"
#include "Random.h"
#include "AgentPack.h"
#include "MessagerAgent.h"
#include "VideoAgent.h"
#include "InputAgent.h"
#include "TimerAgent.h"
#include "GameAgent.h"
#include "SoundAgent.h"
#include "SDLSoundAgent.h"
#include "DummySoundAgent.h"
#include "ScriptAgent.h"
#include "OptionAgent.h"
#include "ResSoundAgent.h"
#include "ResImageAgent.h"

#include "SimpleMsg.h"

#include "SDL.h"

//-----------------------------------------------------------------
Application::Application()
{
    m_quit = false;
    Random::init();

    m_agents = new AgentPack();
    //NOTE: MessagerAgent is added by AgentPack
    //NOTE: creating order is not significant, names are significant,
    // like rc.d scripts
    m_agents->addAgent(new ScriptAgent());
    m_agents->addAgent(new OptionAgent());
    m_agents->addAgent(new VideoAgent());
    m_agents->addAgent(new InputAgent());
    m_agents->addAgent(new TimerAgent());
    m_agents->addAgent(new GameAgent());

    m_agents->addAgent(new ResSoundAgent());
    m_agents->addAgent(new ResImageAgent());
    m_agents->addAgent(new SDLSoundAgent());
}
//-----------------------------------------------------------------
Application::~Application()
{
    delete m_agents;
}
//-----------------------------------------------------------------
    void
Application::init(int argc, char *argv[])
{
    //TEST: args
    ExInfo exinfo = ExInfo("application init");
    exinfo.addInfo("argc", argc);
    for (int i = 0; i < argc; i++) {
        exinfo.addInfo("arg", argv[i]);
    }
    LOG_DEBUG(exinfo);

    MessagerAgent::agent()->addListener(this);
    m_agents->init(Name::VIDEO_NAME);
    OptionAgent::agent()->parseCmdOpt(argc, argv);

    m_agents->init();
    //TODO: allow no sound
    /*
    if (false == OptionAgent::agent()->getAsInt("sound")) {
        m_agents->updateAgent(new DummySoundAgent());
    }
    */
}
//-----------------------------------------------------------------
    void
Application::run()
{
    while (false == m_quit) {
        m_agents->update();
    }
}
//-----------------------------------------------------------------
    void
Application::shutdown()
{
    LOG_DEBUG(ExInfo("application shutdown"));
    m_agents->shutdown();
}
//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - quit ... application quit
 * - inc_loglevel ... inc loglevel by 1 (max LEVEL_DEBUG)
 * - dec_loglevel ... dec loglevel by 1 (min LEVEL_ERROR)
 */
    void
Application::receiveSimple(const SimpleMsg *msg)
{
    if ("quit" == msg->getName()) {
        m_quit = true;
    }
    else if ("inc_loglevel" == msg->getName()) {
        int level = Log::getLogLevel() + 1;
        if (level <= Log::LEVEL_DEBUG) {
            Log::setLogLevel(level);
        }
    }
    else if ("dec_loglevel" == msg->getName()) {
        int level = Log::getLogLevel() - 1;
        if (level >= Log::LEVEL_ERROR) {
            Log::setLogLevel(Log::getLogLevel() - 1);
        }
    }
    else {
        LOG_WARNING(ExInfo("unknown msg")
                .addInfo("msg", msg->toString()));
    }
}


