#ifndef HEADER_LEVEL_H
#define HEADER_LEVEL_H

class Cube;
class Unit;
class Room;
class Actor;

#include "V2.h"
#include "Path.h"
#include "Planner.h"

#include <string>

/**
 * Game level with room.
 */
class Level : public Planner {
    private:
        std::string m_codename;
        Path m_datafile;
        Room *m_room;
        int m_restartCounter;
        std::string m_desc;
        std::string m_loadedMoves;
        int m_loadSpeed;
    private:
        void cleanRoom();
        bool nextPlayerAction();
        bool nextPlanAction();
        bool nextLoadAction();

        void checkRoom();
        void registerGameFuncs();
    public:
        Level(const std::string &codename, const Path &datafile);
        ~Level();
        void setDesc(const std::string &desc) { m_desc = desc; }
        void runLevel();
        void activate();
        void deactivate();

        int getRestartCounter() const { return m_restartCounter; }
        bool nextAction();
        void updateLevel();

        void createRoom(int w, int h, const Path &picture);
        int addModel(Cube *model, Unit *newUnit);
        virtual Actor *getActor(int model_index);
        Cube *getModel(int model_index);
        Cube *askField(const V2 &loc);

        void saveSolution();
        void saveGame(const std::string &models);
        void loadGame(const std::string &moves);

        bool action_restart();
        bool action_move(char symbol);
        bool action_save();
        bool action_load();

        void addSound(const std::string &name, const Path &file);
        void switchFish();
        int getCycles();
};

#endif
