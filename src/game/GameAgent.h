#ifndef HEADER_GAMEAGENT_H
#define HEADER_GAMEAGENT_H

class Room;
class Driver;

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
        int m_lockPhases;
    private:
        Driver *createDriver(const std::string &kind,
            Cube::eWeight *out_weight, Cube::eWeight *out_power,
            bool *out_alive);
        void registerGameFuncs();
        void keyBinding();
        void checkRoom();
    protected:
        virtual void own_init();
        virtual void own_update();
        virtual void own_shutdown();
    public:
        void createRoom(int w, int h, const Path &picture);
        int addModel(const std::string &kind,
                const V2 &loc, const Path &picture,
                const std::string &shape);
        void model_addAnim(int model_index,
                const std::string &name, const std::string &picture);
        void model_setAnim(int model_index,
                const std::string &name, int phase);

        void ensurePhases(int count);
        int getEnsuredPhases() { return m_lockPhases; }
};

#endif
