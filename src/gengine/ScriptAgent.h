#ifndef HEADER_SCRIPTAGENT_H
#define HEADER_SCRIPTAGENT_H

#include "BaseAgent.h"
#include "Name.h"
#include "Path.h"

#include <string>

extern "C" {
#include "lua.h"
}

/**
 * Scripting.
 */
class ScriptAgent : public BaseAgent {
    AGENT(ScriptAgent, Name::SCRIPT_NAME);
    private:
        lua_State *m_state;
    private:
        void callStack(int error);
        void doFile(const Path &file);
        void doString(const std::string &input);
    protected:
        virtual void own_init();
        virtual void own_shutdown();

        virtual void receiveString(const StringMsg *msg);
    public:
        void registerFunc(const char *name, lua_CFunction func);
};

#endif
