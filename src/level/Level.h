#ifndef HEADER_LEVEL_H
#define HEADER_LEVEL_H

class Cube;
class Unit;
class PhaseLocker;
class Picture;
class DemoMode;
class LevelStatus;
class KeyStroke;
class LevelStatus;
class LevelScript;
class CommandQueue;
class Command;

#include "Path.h"
#include "GameState.h"

#include <string>

/**
 * Game level with room.
 */
class Level : public GameState {
    private:
        enum eRoomState {
            ROOM_RUNNING,
            ROOM_COMPLETE,
            ROOM_WRONG
        };

        int m_depth;
        std::string m_desc;
        std::string m_codename;
        Path m_datafile;
        PhaseLocker *m_locker;
        LevelScript *m_levelScript;
        CommandQueue *m_show;
        int m_restartCounter;
        int m_loadSpeed;
        std::string m_loadedMoves;
        int m_countdown;
        //TODO: move eRoomState, satisfyRoom(), countDown() to the LevelStatus
        eRoomState m_roomState;
        LevelStatus *m_levelStatus;
    private:
        bool nextAction();
        void updateLevel();
        void finishLevel();
        bool satisfyRoom(bool room_complete);
        bool countDown();
        bool nextPlayerAction();
        bool nextShowAction();
        bool nextLoadAction();
        void saveSolution();
    protected:
        virtual void own_initState();
        virtual void own_updateState();
        virtual void own_pauseState();
        virtual void own_resumeState();
        virtual void own_cleanState();
    public:
        Level(const std::string &codename, const Path &datafile, int depth);
        ~Level();
        virtual const char *getName() const { return "state_level"; };
        void setDesc(const std::string &desc) { m_desc = desc; }
        void fillStatus(LevelStatus *status) { m_levelStatus = status; }

        void saveGame(const std::string &models);
        void loadGame(const std::string &moves);

        bool action_restart();
        bool action_move(char symbol);
        bool action_save();
        bool action_load();

        void switchFish();
        void controlEvent(const KeyStroke &stroke);

        int getRestartCounter() const { return m_restartCounter; }
        int getDepth() const { return m_depth; }

        void createRoom(int w, int h, const Path &picture);
        void newDemo(Picture *new_bg, const Path &demofile);

        bool isShowing() const;
        void interruptShow();
        void planShow(Command *new_command);
};

#endif
