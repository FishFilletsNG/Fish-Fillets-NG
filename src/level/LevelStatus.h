#ifndef HEADER_LEVELSTATUS_H
#define HEADER_LEVELSTATUS_H

class ScriptState;
class DemoMode;

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
        std::string m_poster;
        std::string m_savedMoves;
    private:
        static int script_status_readMoves(lua_State *L) throw();
        ScriptState *createScript();
        std::string getSolutionFilename();
    public:
        LevelStatus() { m_complete = false; m_wasRunning = false; }
        void prepareRun(const std::string &codename,
                const std::string &poster);
        DemoMode *createPoster() const;

        void setComplete() { m_complete = true; }
        bool isComplete() const { return m_complete; }
        void setRunning(bool value) { m_wasRunning = value; }
        bool wasRunning() const { return m_wasRunning; }

        void readMoves(const std::string &moves);
        std::string readSolvedMoves();
        void writeSolvedMoves(const std::string &moves);

        static std::string getSolutionFilename(const std::string &codename);
};

#endif
