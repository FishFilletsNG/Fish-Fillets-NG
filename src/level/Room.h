#ifndef HEADER_ROOM_H
#define HEADER_ROOM_H

class Path;
class Field;
class FinderAlg;
class WavyPicture;
class Controls;
class KeyStroke;
class MouseStroke;
class Unit;
class ResSoundPack;
class PhaseLocker;
class Planner;
class View;
class Decor;
class InputProvider;
class StepCounter;

#include "Drawable.h"
#include "Cube.h"

#include <string>

/**
 * Room with level.
 */
class Room : public Drawable {
    private:
        WavyPicture *m_bg;
        std::string m_bgFilename;
        Field *m_field;
        FinderAlg *m_finder;
        ResSoundPack *m_soundPack;
        Controls *m_controls;
        PhaseLocker *m_locker;
        Planner *m_levelScript;
        View *m_view;
        Cube::t_models m_models;
        Cube::eAction m_lastAction;
        int m_startTime;
        bool m_fastFalling;
    private:
        void prepareRound();
        bool fallout(bool interactive=true);
        bool falldown(bool interactive=true);
        void playImpact(Cube::eWeight impact);
        void playDead(Cube *model);
        bool isFresh() const { return m_lastAction == Cube::ACTION_NO; }
    public:
        Room(int w, int h, const std::string &picture,
                PhaseLocker *locker, Planner *levelScript);
        ~Room();
        void setWaves(float amplitude, float periode, float speed);
        void addDecor(Decor *new_decor);
        void setFastFalling(bool value) { m_fastFalling = value; }

        int addModel(Cube *new_model, Unit *new_unit);
        Cube *getModel(int model_index);
        Cube *askField(const V2 &loc);

        bool beginFall(bool interactive=true);
        void nextRound(const InputProvider *input);
        void finishRound(bool interactive=true);


        void switchFish();
        void controlEvent(const KeyStroke &stroke);
        void controlMouse(const MouseStroke &button);

        void loadMove(char move);
        bool makeMove(char move);
        bool cannotMove() const;
        bool isSolvable() const;
        bool isSolved() const;
        bool isFalling() const { return m_lastAction == Cube::ACTION_FALL; }
        void checkActive();
        void unBusyUnits();
        const StepCounter *stepCounter() const;
        void setMoves(const std::string &moves);

        int getW() const;
        int getH() const;
        int getCycles() const;
        void addSound(const std::string &name, const Path &file);
        void playSound(const std::string &name, int volume=100);

        void setScreenShift(const V2 &shift);
        void changeBg(const std::string &picture);
        std::string getBg() const { return m_bgFilename; }
        virtual void drawOn(SDL_Surface *screen);
};

#endif
