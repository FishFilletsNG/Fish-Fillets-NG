#ifndef HEADER_ROOM_H
#define HEADER_ROOM_H

class Field;
class Picture;
class Controls;
class KeyStroke;
class Unit;
class ResSoundPack;
class PhaseLocker;
class Planner;
class View;

#include "NoCopy.h"
#include "Path.h"
#include "Cube.h"

#include <string>

/**
 * Room with level.
 */
class Room : public NoCopy {
    private:
        Picture *m_bg;
        Field *m_field;
        ResSoundPack *m_soundPack;
        Controls *m_controls;
        PhaseLocker *m_locker;
        Planner *m_levelScript;
        View *m_view;
        Cube::t_models m_models;
        Cube::eWeight m_impact;
        int m_startTime;
        bool m_fresh;
    private:
        void killPlan();
        void prepareRound();
        bool falldown();
        void playImpact();
        void playDead(Cube *model);
    public:
        Room(int w, int h, const Path &picture,
                PhaseLocker *locker, Planner *levelScript);
        ~Room();
        void activate();
        void deactivate();

        int addModel(Cube *new_model, Unit *new_unit);
        Cube *getModel(int model_index);
        Cube *askField(const V2 &loc);

        bool beginFall(bool sound=true);
        bool nextRound();
        bool finishRound(bool anim=true);


        void switchFish();
        void controlEvent(const KeyStroke &stroke);
        std::string getMoves() const;
        bool loadMove(char move);
        bool makeMove(char move);
        bool cannotMove();
        bool isSolvable();

        int getW() const;
        int getH() const;
        int getCycles() const;
        void addSound(const std::string &name, const Path &file);
        void playSound(const std::string &name, int priority);
};

#endif
