#ifndef HEADER_SCRIPTAGENT_H
#define HEADER_SCRIPTAGENT_H

class Path;
class ScriptState;

#include "BaseAgent.h"
#include "Name.h"

#include <string>

extern "C" {
#include "lua.h"
}

/**
 * Global scripting.
 * It is used by OptionAgent and debug console.
 */
class ScriptAgent : public BaseAgent {
    AGENT(ScriptAgent, Name::SCRIPT_NAME);
    private:
        ScriptState *m_state;
    protected:
        virtual void own_init();
        virtual void own_shutdown();
    public:
        void registerFunc(const char *name, lua_CFunction func);
        void doFile(const Path &file);

        virtual void receiveString(const StringMsg *msg);
};

#endif
