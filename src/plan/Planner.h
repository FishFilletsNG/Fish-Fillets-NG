#ifndef HEADER_PLANNER_H
#define HEADER_PLANNER_H

class Path;
class ScriptState;
class CommandQueue;
class Actor;

#include "NoCopy.h"

#include "SDL.h"
#include <string>

/**
 * Action planner.
 */
class Planner : public NoCopy {
    private:
        CommandQueue *m_plan;
    protected:
        ScriptState *m_script;
    private:
        void registerScriptFuncs();
    public:
        Planner();
        virtual ~Planner();

        void scriptInclude(const Path &filename);
        void scriptDo(const std::string &input);
        bool satisfyPlan();
        void interruptPlan();

        void planAction(int funcRef);
        bool isPlanning() const;
};

#endif
