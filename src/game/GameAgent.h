#ifndef HEADER_GAMEAGENT_H
#define HEADER_GAMEAGENT_H

class Room;
class Unit;
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
        int m_restartCounter;
        std::string m_codename;
    private:
        void newLevel(bool restart=false);
        void clearRoom();
        std::string getNextLevel(bool restart=false);
        void registerGameFuncs();
        void checkRoom();

        Unit *createUnit(const std::string &kind,
            Cube::eWeight *out_weight, Cube::eWeight *out_power,
            bool *out_alive);
        void keyBinding();
        void saveLevel();
        void loadLevel();
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
        Cube *askField(const V2 &loc);

        void ensurePhases(int count);
        int getEnsuredPhases() const { return m_lockPhases; }
        int getRestartCounter() const { return m_restartCounter; }

        void saveGame(const std::string &models);

        virtual void receiveSimple(const SimpleMsg *msg);
};

#endif
