#ifndef HEADER_SCRIPTAGENT_H
#define HEADER_SCRIPTAGENT_H

class Path;
class ScriptState;

#include "BaseAgent.h"
#include "Name.h"
#include "Scripter.h"

#include <string>

extern "C" {
#include "lua.h"
}

/**
 * Global scripting.
 * It is used by OptionAgent and debug console.
 */
class ScriptAgent : public BaseAgent, public Scripter {
    AGENT(ScriptAgent, Name::SCRIPT_NAME);
    protected:
        virtual void own_init();
    public:
        void registerFunc(const char *name, lua_CFunction func);

        virtual void receiveString(const StringMsg *msg);
};

#endif
