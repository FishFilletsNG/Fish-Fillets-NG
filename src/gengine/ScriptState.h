#ifndef HEADER_SCRIPTSTATE_H
#define HEADER_SCRIPTSTATE_H

#include "Path.h"

#include <string>

extern "C" {
#include "lua.h"
}

/**
 * Independent script state.
 */
class ScriptState {
    private:
        lua_State *m_state;
    private:
        void callStack(int error, int params=0, int returns=0);
    public:
        ScriptState();
        ~ScriptState();

        void doFile(const Path &file);
        void doString(const std::string &input);
        bool callCommand(int funcRef, int param);
        void unref(int funcRef);

        void registerFunc(const char *name, lua_CFunction func);
        void registerLeader(void *leader);
};

#endif
