/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
/**
 *
 * \mainpage
 * 
 * GenGine is work name for our Game Engine.
 * Engine architecture composes of agents responsible for specific action.
 * 
 * \section AgentPack
 * 
 * AgentPack groups agents which have something to do. It inits agents, update them and shutdown them at the end.
 * 
 * Every agent has init(), update() and shutdown() methods. These methods are empty by default.
 * 
 * Available agents:
 *  - MessagerAgent - allows sending messages to agent and other listeners
 *  - ScriptAgent - makes scripting possible
 *  - OptionAgent - contains global game options
 *  - VideoAgent - inits video mode and calls drawers drawOn() method
 *  - SoundAgent - plays music and sound
 *  - TimerAgent - sleeps constant delay = makes stable FPS
 *  - InputAgent - reads input events and handles keystrokes
 *  - SubTitleAgent - scrolls subtitles on display
 *  - GameAgent - updates game states
 * 
 * AgentPack will call agents ordered by their names. See Name.cpp for names.
 * AgentPack::init() calls init() on all agents. AgentPack::update() calls
 * update() on all agents. AgentPack::shutdown() calls shutdown() on all agents.
 * 
 * 
 * \subsection mini_app Minimal application
 * \code
 * 
 * AgentPack *agents = new AgentPack();
 * try {
 *     agents->addAgent(new ScriptAgent());
 *     agents->addAgent(new OptionAgent());
 *     agents->init();
 * 
 *     while (true) {
 *         agents->update();
 *     }
 * }
 * catch (BaseException &e) {
 *     LOG_ERROR(e.info());
 *     agents->shutdown();
 * }
 * 
 * delete agents;
 * \endcode
 * 
 * 
 * \note
 * MessagerAgent is always included in AgentPack.
 * It is need to register every agent as listener during AgentPack::addAgent().
 * 
 * \section BaseAgent
 * 
 * Every agent inherits from BaseAgent.
 * 
 * Rules for agents:
 *  -# Agent must not call other agents in his constructor.
 *  -# Agent can call only agents with lower names and oneself in his init().
 *  -# Agent can call only agents with higher names
 *     and oneself in his shutdown().
 * 
 * For example,
 * agent "30video" can ask agent "20option" about screen_width in his init().
 * 
 * \section Communication
 * 
 * Every agent has static method agent(). It asks AgentPack::getAgent()
 * for agent instance.
 * 
 * agent() method is made by AGENT(TYPE, NAME) macro. This allows us
 * to obtain agent of preferred type and is not need
 * to write agent() method for every agent.
 * 
 * \subsection comm-example How to get screen_width
 * \code 
 * OptionAgent::agent()->getAsInt("screen_width");
 * \endcode
 * 
 */

#include "SDL.h"

#include "Log.h"
#include "Application.h"
#include "HelpException.h"
#include "BaseException.h"

#include <stdio.h> //printf

//-----------------------------------------------------------------
    int
main(int argc, char *argv[])
{
    try {
        Application app;

        try {
            app.init(argc, argv);
            app.run();
        }
        catch (HelpException &e) {
            printf("%s\n", e.what());
        }
        catch (BaseException &e) {
            LOG_ERROR(e.info());
        }
        app.shutdown();
        return 0;
    }
    catch (BaseException &e) {
        LOG_ERROR(e.info());
    }
    catch (std::exception &e) {
        LOG_ERROR(ExInfo("std::exception")
                .addInfo("what", e.what()));
    }
    catch (...) {
        LOG_ERROR(ExInfo("unknown exception"));
    }

    return 1;
}

