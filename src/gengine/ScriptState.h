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
        void callStack(int error);
    public:
        ScriptState();
        ~ScriptState();

        void doFile(const Path &file);
        void doString(const std::string &input);

        void registerFunc(const char *name, lua_CFunction func);
};

#endif
