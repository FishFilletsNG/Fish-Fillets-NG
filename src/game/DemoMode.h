#ifndef HEADER_DEMOMODE_H
#define HEADER_DEMOMODE_H

class Actor;
class Picture;

#include "Planner.h"
#include "Path.h"

#include <map>

/**
 * Graphic demo.
 */
class DemoMode : public Planner {
    private:
        Picture *m_bg;
        Picture *m_display;
        typedef std::map<int,Actor*> t_actors;
        t_actors m_actors;
    public:
        DemoMode();
        virtual ~DemoMode();

        void runDemo(Picture *bg, const Path &demoscript);

        virtual Actor *getActor(int model_index);
        bool action_display(Picture *picture);
};

#endif
