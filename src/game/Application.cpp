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
#include "DialogAgent.h"
#include "SubTitleAgent.h"

#include "SimpleMsg.h"
#include "StringMsg.h"

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

    m_agents->addAgent(new ResSoundAgent());
    m_agents->addAgent(new ResImageAgent());
    m_agents->addAgent(new InputAgent());

    m_agents->addAgent(new DialogAgent());
    m_agents->addAgent(new SubTitleAgent());
    m_agents->addAgent(new GameAgent());

    m_agents->addAgent(new TimerAgent());
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
    MessagerAgent::agent()->addListener(this);
    m_agents->init(Name::VIDEO_NAME);
    OptionAgent::agent()->parseCmdOpt(argc, argv);
    //TODO: better setting sound on/off
    if (OptionAgent::agent()->getAsInt("sound", 1)) {
        m_agents->addAgent(new SDLSoundAgent());
    }
    else {
        m_agents->addAgent(new DummySoundAgent());
    }

    prepareLogLevel();
    m_agents->init();
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
 * Set loglevel according option.
 * Prepare to change.
 */
void
Application::prepareLogLevel()
{
    OptionAgent *options = OptionAgent::agent();
    StringMsg *event = new StringMsg(this, "param_changed", "loglevel");
    options->addWatcher("loglevel", event);
    options->setParam("loglevel",
            options->getAsInt("loglevel", Log::getLogLevel()));
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
    if (msg->equalsName("quit")) {
        m_quit = true;
    }
    else if (msg->equalsName("inc_loglevel")) {
        int level = Log::getLogLevel() + 1;
        if (level <= Log::LEVEL_DEBUG) {
            OptionAgent::agent()->setParam("loglevel", level);
        }
    }
    else if (msg->equalsName("dec_loglevel")) {
        int level = Log::getLogLevel() - 1;
        if (level >= Log::LEVEL_ERROR) {
            OptionAgent::agent()->setParam("loglevel", level);
        }
    }
    else {
        LOG_WARNING(ExInfo("unknown msg")
                .addInfo("msg", msg->toString()));
    }
}
//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - param_changed(loglevel) ... set loglevel
 */
    void
Application::receiveString(const StringMsg *msg)
{
    if (msg->equalsName("param_changed")) {
        std::string param = msg->getValue();
        if ("loglevel" == param) {
            Log::setLogLevel(OptionAgent::agent()->getAsInt("loglevel"));
        }
    }
    else {
        LOG_WARNING(ExInfo("unknown msg")
                .addInfo("msg", msg->toString()));
    }
}

