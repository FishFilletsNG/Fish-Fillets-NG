#ifndef HEADER_DEMOMODE_H
#define HEADER_DEMOMODE_H

class Picture;
class DemoInput;

#include "Path.h"
#include "GameState.h"
#include "Planner.h"
#include "Drawable.h"

/**
 * Graphic demo.
 */
class DemoMode : public Planner, public GameState, public Drawable {
    private:
        int m_oldLimitY;
        Path m_demoscript;
        SDL_Surface *m_surfaceBuffer;
        Picture *m_display;
    protected:
        virtual void own_initState();
        virtual void own_updateState();
        virtual void own_pauseState() {}
        virtual void own_resumeState() {}
        virtual void own_cleanState();
    public:
        DemoMode(const Path &demoscript);
        virtual ~DemoMode();
        virtual const char *getName() const { return "state_demo"; };

        bool action_display(Picture *picture);
        virtual void drawOn(SDL_Surface *screen);
};

#endif
