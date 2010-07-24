#ifndef HEADER_GAMEAGENT_H
#define HEADER_GAMEAGENT_H

class StateManager;

#include "BaseAgent.h"
#include "Name.h"

#include <string>

/**
 * Create game.
 * GameAgent uses StateManager to manage
 * WorldMap, Pedometer, Level and DemoMode.
 */
class GameAgent : public BaseAgent {
    AGENT(GameAgent, Name::GAME_NAME);
    private:
        StateManager *m_manager;
    private:
        void replaySolution(const std::string &codename);
        void keyBinding();
    protected:
        virtual void own_init();
        virtual void own_update();
        virtual void own_shutdown();
};

#endif
