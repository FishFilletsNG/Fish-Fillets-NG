#ifndef HEADER_LEVEL_H
#define HEADER_LEVEL_H

class Cube;
class Unit;
class DescFinder;
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
        static const int SPEED_REPLAY = 1;
        enum eRoomState {
            ROOM_RUNNING,
            ROOM_COMPLETE,
            ROOM_WRONG
        };

        int m_depth;
        const DescFinder *m_desc;
        std::string m_codename;
        Path m_datafile;
        PhaseLocker *m_locker;
        bool m_newRound;
        LevelScript *m_levelScript;
        CommandQueue *m_show;
        int m_restartCounter;
        int m_loadSpeed;
        bool m_replayMode;
        std::string m_loadedMoves;
        int m_countdown;
        //TODO: move eRoomState, satisfyRoom(), countDown() to the LevelStatus
        eRoomState m_roomState;
        LevelStatus *m_levelStatus;
    private:
        void initScreen();
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
        virtual void own_noteFg();
    public:
        Level(const std::string &codename, const Path &datafile, int depth);
        ~Level();
        virtual const char *getName() const { return "state_level"; };
        void fillDesc(const DescFinder *desc) { m_desc = desc; }
        void fillStatus(LevelStatus *status) { m_levelStatus = status; }

        void saveGame(const std::string &models);
        void loadGame(const std::string &moves);
        void loadReplay(const std::string &moves);

        bool action_restart();
        bool action_move(char symbol);
        bool action_save();
        bool action_load();

        void switchFish();
        void controlEvent(const KeyStroke &stroke);

        int getRestartCounter() const { return m_restartCounter; }
        int getDepth() const { return m_depth; }
        bool isNewRound() const { return m_newRound; }

        void createRoom(int w, int h, const Path &picture);
        void setRoomWaves(double amplitude, double periode, double speed);
        void newDemo(Picture *new_bg, const Path &demofile);
        void pushState(GameState *new_state);

        bool isLoading() const;
        bool isShowing() const;
        void interruptShow();
        void planShow(Command *new_command);
};

#endif
