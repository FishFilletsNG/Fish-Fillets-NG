#ifndef HEADER_ROOM_H
#define HEADER_ROOM_H

class Field;
class Picture;
class Controls;
class Unit;
class ResSoundPack;

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
        Cube::t_models m_models;
        Cube::eWeight m_impact;
        bool m_fresh;
    private:
        void prepareRound();
        bool falldown();
        void playImpact();
        void playDead(Cube *model);
    public:
        Room(int w, int h, const Path &picture);
        ~Room();

        int addModel(Cube *model, Unit *newUnit);
        Cube *getModel(int model_index);
        Cube *askField(const V2 &loc);

        bool beginFall(bool sound=true);
        bool nextRound();
        bool finishRound(bool anim=true);

        void switchFish();
        std::string getMoves() const;
        bool loadMove(char move);
        bool makeMove(char move);

        int getW() const;
        int getH() const;
        void addSound(const std::string &name, const Path &file);
};

#endif
