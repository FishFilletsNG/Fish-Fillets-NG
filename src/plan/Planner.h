#ifndef HEADER_PLANNER_H
#define HEADER_PLANNER_H

class Path;
class CommandQueue;
class DialogStack;

#include "Scripter.h"

#include "SDL.h"
#include <string>

/**
 * Action planner.
 */
class Planner : public Scripter {
    private:
        CommandQueue *m_plan;
        DialogStack *m_dialogs;
    private:
        void registerScriptFuncs();
    public:
        Planner();
        virtual ~Planner();

        bool satisfyPlan();
        void killPlan();
        virtual void interruptPlan();

        void planAction(int funcRef);
        bool isPlanning() const;
        DialogStack *dialogs() { return m_dialogs; }
};

#endif
