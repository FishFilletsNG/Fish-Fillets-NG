#ifndef HEADER_PEDOMETER_H
#define HEADER_PEDOMETER_H

class Level;
class LevelStatus;
class LayeredPicture;
class Picture;

#include "Drawable.h"
#include "GameState.h"

#include "SDL.h"
#include <string>

/**
 * Pedometer with tree buttons.
 */
class Pedometer : public GameState, public Drawable {
    private:
        Picture *m_bg;
        LayeredPicture *m_rack;
        SDL_Surface *m_numbers;
        LevelStatus *m_status;
        Level *m_level;
        Uint32 m_activeMask;
        Uint32 m_maskRun;
        Uint32 m_maskReplay;
        Uint32 m_maskCancel;
        std::string m_solution;
        int m_meterPhase;
    private:
        void prepareBg();
        void prepareRack();

        void watchCursor();
        void runLevel();
        void runReplay();

        void drawNumbers(SDL_Surface *screen, int value);
        void drawNumber(SDL_Surface *screen, int x, int y, int shiftY);
    protected:
        virtual void own_initState();
        virtual void own_updateState();
        virtual void own_pauseState() {}
        virtual void own_resumeState() {}
        virtual void own_cleanState() {}
    public:
        Pedometer(LevelStatus *status, Level *level);
        virtual ~Pedometer();
        virtual const char *getName() const { return "state_pedometer"; };

        void runSelected();
        virtual void drawOn(SDL_Surface *screen);
        void receiveString(const StringMsg *msg);
};

#endif
