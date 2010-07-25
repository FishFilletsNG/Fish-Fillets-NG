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
#include "Path.h"
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
#include "SubTitleAgent.h"
#include "ResourceException.h"
#include "OptionParams.h"
#include "Font.h"

#include "SimpleMsg.h"
#include "StringMsg.h"

#include "SDL.h"
#include <stdio.h> // for fflush, stdout

//-----------------------------------------------------------------
Application::Application()
{
    m_quit = false;
    Random::init();
    Font::init();

    m_agents = new AgentPack();
    //NOTE: MessagerAgent is added by AgentPack
    //NOTE: creating order is not significant, names are significant,
    // like rc.d scripts
    m_agents->addAgent(new ScriptAgent());
    m_agents->addAgent(new OptionAgent());
    m_agents->addAgent(new VideoAgent());

    m_agents->addAgent(new InputAgent());

    m_agents->addAgent(new SubTitleAgent());
    m_agents->addAgent(new GameAgent());

    m_agents->addAgent(new TimerAgent());
}
//-----------------------------------------------------------------
Application::~Application()
{
    delete m_agents;
    Font::shutdown();
}
//-----------------------------------------------------------------
    void
Application::init(int argc, char *argv[])
{
    MessagerAgent::agent()->addListener(this);
    m_agents->init(Name::VIDEO_NAME);
    prepareLogLevel();
    prepareOptions(argc, argv);
    customizeGame();

    m_agents->init(Name::TIMER_NAME);
    addSoundAgent();

    m_agents->init();
}
//-----------------------------------------------------------------
    void
Application::run()
{
    while (!m_quit) {
        m_agents->update();
    }
}
//-----------------------------------------------------------------
    void
Application::shutdown()
{
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
    options->setDefault("loglevel", Log::getLogLevel());
}
//-----------------------------------------------------------------
    void
Application::prepareOptions(int argc, char *argv[])
{
    OptionParams params;
    params.addParam("loglevel", OptionParams::TYPE_NUMBER,
            "Debug with loglevel 7 (default=6)");
    params.addParam("systemdir", OptionParams::TYPE_PATH,
            "Path to game data");
    params.addParam("userdir", OptionParams::TYPE_PATH,
            "Path to game data");
    params.addParam("lang", OptionParams::TYPE_STRING,
            "2-letter code (e.g., en, cs, fr, de)");
    params.addParam("speech", OptionParams::TYPE_STRING,
            "Lang for speech");
    params.addParam("subtitles", OptionParams::TYPE_BOOLEAN,
            "Enable subtitles");
    params.addParam("fullscreen", OptionParams::TYPE_BOOLEAN,
            "Turn fullscreen on/off");
    params.addParam("show_steps", OptionParams::TYPE_BOOLEAN,
            "Show a step counter in levels");
    params.addParam("sound", OptionParams::TYPE_BOOLEAN,
            "Turn sound on/off");
    params.addParam("volume_sound", OptionParams::TYPE_NUMBER,
            "Sound volume in percentage");
    params.addParam("volume_music", OptionParams::TYPE_NUMBER,
            "Music volume in percentage");
    params.addParam("worldmap", OptionParams::TYPE_STRING,
            "Path to the worldmap file");
    params.addParam("cache_images", OptionParams::TYPE_BOOLEAN,
            "Cache images (default=true)");
    params.addParam("sound_frequency", OptionParams::TYPE_NUMBER,
            "Sound sample rate (default=44100)");
    params.addParam("strict_rules", OptionParams::TYPE_BOOLEAN,
            "Disallow pushing of partially supported objects (default=true)");
    params.addParam("replay_level", OptionParams::TYPE_STRING,
            "Replay the solution for the given level codename");
    OptionAgent::agent()->parseCmdOpt(argc, argv, params);
}
//-----------------------------------------------------------------
/**
 * Run init script.
 * @throws ResourceException when data are not available
 */
    void
Application::customizeGame()
{
    Path initfile = Path::dataReadPath("script/init.lua");
    if (initfile.exists()) {
        ScriptAgent::agent()->scriptInclude(initfile);
    }
    else {
        throw ResourceException(ExInfo("init file not found")
                .addInfo("path", initfile.getNative())
                .addInfo("systemdir",
                    OptionAgent::agent()->getParam("systemdir"))
                .addInfo("userdir",
                    OptionAgent::agent()->getParam("userdir"))
                .addInfo("hint",
                    "try command line option \"systemdir=path/to/data\""));
    }
}
//-----------------------------------------------------------------
/**
 * Choose SDL or Dummy sound agent.
 * Reads 'sound' config option.
 */
    void
Application::addSoundAgent()
{
    //TODO: better setting sound on/off
    //TODO: move to the SoundAgent
    SoundAgent *soundAgent = NULL;
    if (OptionAgent::agent()->getAsBool("sound", true)) {
        soundAgent = new SDLSoundAgent();
        try {
            soundAgent->init();
        }
        catch (BaseException &e) {
            LOG_WARNING(e.info());
            delete soundAgent;
            soundAgent = new DummySoundAgent();
        }
    }
    else {
        soundAgent = new DummySoundAgent();
    }
    m_agents->addAgent(soundAgent);
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
    else if (msg->equalsName("flush_stdout")) {
        fflush(stdout);
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

