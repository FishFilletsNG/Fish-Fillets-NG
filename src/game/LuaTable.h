#ifndef HEADER_LUATABLE_H
#define HEADER_LUATABLE_H

#include "NoCopy.h"

#include <string>

extern "C" {
#include "lua.h"
}

/**
 * Table from Lua in C.
 */
class LuaTable {
    private:
        lua_State *m_state;
        int m_stackIndex;
    public:
        LuaTable(lua_State *state, int stackIndex);
        int getStackIndex() const { return m_stackIndex; }

        std::string getString(const std::string &key) const;
        int getInt(const std::string &key) const;
        bool getBool(const std::string &key) const;
        LuaTable pushTable(const std::string &key) const;
};

#endif
