#ifndef HEADER_LEVEL_H
#define HEADER_LEVEL_H

class Cube;
class Unit;
class Room;
class ScriptState;
class CommandQueue;

#include "V2.h"
#include "Path.h"
#include "NoCopy.h"

#include <string>

/**
 * Game level with room.
 */
class Level : public NoCopy {
    private:
        std::string m_codename;
        Path m_datafile;
        ScriptState *m_script;
        Room *m_room;
        int m_restartCounter;
        std::string m_loadedMoves;
        int m_loadSpeed;
        CommandQueue *m_plan;
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

        int getRestartCounter() const { return m_restartCounter; }
        bool nextAction();
        void updateLevel();

        void scriptInclude(const Path &filename);
        void createRoom(int w, int h, const Path &picture);
        int addModel(Cube *model, Unit *newUnit);
        Cube *getModel(int model_index);
        Cube *askField(const V2 &loc);

        void saveGame(const std::string &models);
        void loadGame(const std::string &moves);

        void planAction(int funcRef);
        void interruptPlan();
        bool isPlanning() const;
        bool action_restart();
        bool action_move(char symbol);
        bool action_save();
        bool action_load();

        void addSound(const std::string &name, const Path &file);
        void switchFish();
};

#endif
