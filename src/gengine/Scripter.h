#ifndef HEADER_SCRIPTER_H
#define HEADER_SCRIPTER_H

class Path;
class ScriptState;

#include "NoCopy.h"

#include <string>

/**
 * Base class using a script.
 */
class Scripter : public NoCopy {
    protected:
        ScriptState *m_script;
    public:
        Scripter();
        virtual ~Scripter();

        void scriptInclude(const Path &filename);
        void scriptDo(const std::string &input);
};

#endif
