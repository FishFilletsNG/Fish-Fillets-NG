#ifndef HEADER_DEMOMODE_H
#define HEADER_DEMOMODE_H

class Actor;
class Picture;
class DemoInput;

#include "GameState.h"
#include "Planner.h"

#include <map>

/**
 * Graphic demo.
 */
class DemoMode : public Planner, public GameState {
    private:
        Picture *m_bg;
        Picture *m_display;
        typedef std::map<int,Actor*> t_actors;
        t_actors m_actors;
    protected:
        virtual void own_initState() {}
        virtual void own_updateState();
        virtual void own_pauseState() {}
        virtual void own_resumeState() {}
        virtual void own_cleanState();
    public:
        DemoMode();
        virtual ~DemoMode();
        virtual const char *getName() const { return "state_demo"; };

        void runDemo(Picture *bg, const Path &demoscript);

        virtual Actor *getActor(int model_index);
        bool action_display(Picture *picture);
};

#endif
