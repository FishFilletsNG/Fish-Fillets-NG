#ifndef HEADER_APPLICATION_H
#define HEADER_APPLICATION_H

class AgentPack;

#include "NoCopy.h"
#include "BaseListener.h"
#include "Name.h"

/**
 * Main application
 */
class Application : public NoCopy, public BaseListener {
    private:
        AgentPack *m_agents;
        bool m_quit;

    public:
        Application();
        virtual ~Application();
        virtual const std::string &getName() const { return Name::APP_NAME; }

        void init(int argc, char *argv[]);
        void run();
        void shutdown();

        virtual void receiveSimple(const SimpleMsg *msg);
};

#endif
