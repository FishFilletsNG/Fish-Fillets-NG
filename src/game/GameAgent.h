#ifndef HEADER_GAMEAGENT_H
#define HEADER_GAMEAGENT_H

class Planner;
class Level;
class WorldMap;
class DemoMode;
class Picture;

#include "BaseAgent.h"
#include "Name.h"
#include "Path.h"

#include <string>

/**
 * Game.
 */
class GameAgent : public BaseAgent {
    AGENT(GameAgent, Name::GAME_NAME);
    private:
        Level *m_level;
        WorldMap *m_world;
        DemoMode *m_demo;
        int m_lockPhases;
    private:
        void cleanLevel();
        void cleanDemo();
        void newLevel();
        void finishLevel();

        void keyBinding();
    protected:
        virtual void own_init();
        virtual void own_update();
        virtual void own_shutdown();
    public:
        void ensurePhases(int count);
        int getEnsuredPhases() const { return m_lockPhases; }
        void newDemo(Picture *bg, const Path &demofile);

        Planner *leader();
        DemoMode *demo();
        Level *level();
        WorldMap *world() { return m_world; }

        virtual void receiveSimple(const SimpleMsg *msg);
};

#endif
