/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "AgentPack.h"

#include "BaseAgent.h"
#include "LogicException.h"
#include "NameException.h"
#include "MessagerAgent.h"

AgentPack *AgentPack::ms_singleton = NULL;
//-----------------------------------------------------------------
AgentPack::AgentPack()
{
    //NOTE: this is not thread safe
    if (ms_singleton) {
        throw LogicException(ExInfo("AgentPack is singleton"));
    }

    ms_singleton = this;
    //NOTE: MessagerAgent must be the first
    MessagerAgent *messager = new MessagerAgent();
    messager->init();
    addAgent(messager);
}
//-----------------------------------------------------------------
AgentPack::~AgentPack()
{
    t_agents::iterator end = m_agents.end();
    for (t_agents::iterator i = m_agents.begin(); i != end; ++i) {
        delete i->second;
    }
    ms_singleton = NULL;
}
//-----------------------------------------------------------------
/**
 * Insert agent to pack
 * and insert his to listeners too.
 * NOTE: agent->init() is not called yet
 *
 * @param agent agent
 * @throws NameEception if agent with this name already exists.
 */
    void
AgentPack::addAgent(BaseAgent *agent)
{
    std::pair<t_agents::iterator,bool> status =
        m_agents.insert(
                std::pair<std::string,BaseAgent*>(agent->getName(), agent));
    if (!status.second) {
        throw NameException(ExInfo("agent already exists")
                .addInfo("name", agent->getName()));
    }

    MessagerAgent::agent()->addListener(agent);
}
//-----------------------------------------------------------------
/**
 * Remove agent from pack
 * and remove his from listeners too.
 * @param name agent's name
 */
void
AgentPack::removeAgent(const std::string &name)
{
    t_agents::iterator it = m_agents.find(name);
    if (m_agents.end() != it) {
        MessagerAgent::agent()->removeListener(name);
        delete it->second;
        m_agents.erase(it);
    }
}
//-----------------------------------------------------------------
/**
 * Return agent according his name.
 * @param name agent's name
 * @return agent
 * @throws NameException when agent cannot be found
 * @throws LogicException when agent is not initialized
 */
    BaseAgent *
AgentPack::getAgent(const std::string &name)
{
    if (NULL == ms_singleton) {
        throw LogicException(ExInfo("AgentPack is not ready"));
    }

    t_agents::iterator it = ms_singleton->m_agents.find(name);
    if (ms_singleton->m_agents.end() == it) {
        throw NameException(ExInfo("cannot find agent")
                .addInfo("name", name));
    }

    if (!it->second->isInitialized()) {
        throw LogicException(ExInfo("agent is not initialized")
                .addInfo("name", name));
    }
    return it->second;
}
//-----------------------------------------------------------------
/**
 * Init all agents bellow stopAgent.
 * Init all agents when stopAgent is not found.
 */
void
AgentPack::init(const std::string &stopAgent)
{
    t_agents::iterator stop = m_agents.find(stopAgent);

    for (t_agents::iterator i = m_agents.begin(); i != stop; ++i) {
        if (!i->second->isInitialized()) {
            i->second->init();
        }
    }
}
//-----------------------------------------------------------------
void
AgentPack::update()
{
    t_agents::iterator end = m_agents.end();
    for (t_agents::iterator i = m_agents.begin(); i != end; ++i) {
        i->second->update();
    }
}
//-----------------------------------------------------------------
/**
 * Shutdown initialized agents in reverse order.
 */
void
AgentPack::shutdown()
{
    t_agents::reverse_iterator rend = m_agents.rend();
    for (t_agents::reverse_iterator i = m_agents.rbegin();
            i != rend; ++i)
    {
        if (i->second->isInitialized()) {
            i->second->shutdown();
        }
    }
}

