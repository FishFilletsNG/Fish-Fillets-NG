#ifndef HEADER_LEVELSTATUS_H
#define HEADER_LEVELSTATUS_H

class ScriptState;

#include <string>

extern "C" {
#include "lua.h"
}

/**
 * Status of level after a game.
 */
class LevelStatus {
    private:
        bool m_complete;
        std::string m_savedMoves;
    private:
        static int script_status_readMoves(lua_State *L) throw();
        ScriptState *createScript();
    public:
        LevelStatus() { m_complete = false; }
        void setComplete(bool complete) { m_complete = complete; }
        bool isComplete() const { return m_complete; }

        void readMoves(const std::string savedMoves);
        std::string readSolvedMoves(const std::string codename);
        void writeSolvedMoves(const std::string codename,
                const std::string moves);
};

#endif
