#ifndef HEADER_AGENTPACK_H
#define HEADER_AGENTPACK_H

class BaseAgent;

#include "NoCopy.h"
#include <string>

#include <map>

/**
 * List of agents.
 */
class AgentPack : public NoCopy {
    private:
        typedef std::map<std::string,BaseAgent*> t_agents;
        t_agents m_agents;
        static AgentPack *ms_singleton;
    public:
        AgentPack();
        ~AgentPack();

        void addAgent(BaseAgent *agent);
        void removeAgent(const std::string &name);

        static BaseAgent *getAgent(const std::string &name);

        void init(const std::string &stopAgent="");
        void update();
        void shutdown();
};

#endif
