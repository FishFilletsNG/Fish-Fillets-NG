#ifndef HEADER_GAMEAGENT_H
#define HEADER_GAMEAGENT_H

class Room;
class Driver;
class ScriptState;

#include "BaseAgent.h"
#include "Name.h"
#include "Cube.h"
#include "V2.h"
#include "Path.h"

#include <string>

/**
 * Game.
 */
class GameAgent : public BaseAgent {
    AGENT(GameAgent, Name::GAME_NAME);
    private:
        Room *m_room;
        ScriptState *m_script;
        int m_lockPhases;
    private:
        void newLevel();
        void registerGameFuncs();
        void checkRoom();

        Driver *createDriver(const std::string &kind,
            Cube::eWeight *out_weight, Cube::eWeight *out_power,
            bool *out_alive);
        void keyBinding();
    protected:
        virtual void own_init();
        virtual void own_update();
        virtual void own_shutdown();
    public:
        void scriptInclude(const Path &filename);
        void createRoom(int w, int h, const Path &picture);
        int addModel(const std::string &kind,
                const V2 &loc, const std::string &shape);
        Cube *getModel(int model_index);

        void ensurePhases(int count);
        int getEnsuredPhases() { return m_lockPhases; }
};

#endif
