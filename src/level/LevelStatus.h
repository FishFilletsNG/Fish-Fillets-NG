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
        bool m_wasRunning;
        std::string m_codename;
        std::string m_levelName;
        std::string m_savedMoves;
    private:
        static int script_status_readMoves(lua_State *L) throw();
        ScriptState *createScript();
        std::string getSolutionFilename();
    public:
        LevelStatus() { m_complete = false; m_wasRunning = true; }
        void prepareRun(const std::string codename,
                const std::string levelName);
        std::string getLevelName() const { return m_levelName; }

        void setComplete() { m_complete = true; }
        bool isComplete() const { return m_complete; }
        void setRunning() { m_wasRunning = true; }
        bool wasRunning() const { return m_wasRunning; }

        void readMoves(const std::string &moves);
        std::string readSolvedMoves();
        void writeSolvedMoves(const std::string &moves);

        static std::string getSolutionFilename(const std::string &codename);
};

#endif
