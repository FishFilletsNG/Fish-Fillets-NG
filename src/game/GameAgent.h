#ifndef HEADER_GAMEAGENT_H
#define HEADER_GAMEAGENT_H

class Level;

#include "BaseAgent.h"
#include "Name.h"

#include <string>

/**
 * Game.
 */
class GameAgent : public BaseAgent {
    AGENT(GameAgent, Name::GAME_NAME);
    private:
        Level *m_level;
        int m_lockPhases;
    private:
        void cleanLevel();
        void newLevel();
        std::string getNextLevel();

        void keyBinding();
    protected:
        virtual void own_init();
        virtual void own_update();
        virtual void own_shutdown();
    public:
        void ensurePhases(int count);
        int getEnsuredPhases() const { return m_lockPhases; }

        Level *level();

        virtual void receiveSimple(const SimpleMsg *msg);
};

#endif
