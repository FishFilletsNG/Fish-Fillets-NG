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

    private:
        void prepareLogLevel();
        void prepareOptions(int argc, char *argv[]);
        void customizeGame();
        void addSoundAgent();
    public:
        Application();
        virtual ~Application();
        virtual const char *getName() const { return Name::APP_NAME; }

        void init(int argc, char *argv[]);
        void run();
        void shutdown();

        virtual void receiveSimple(const SimpleMsg *msg);
        virtual void receiveString(const StringMsg *msg);
};

#endif
