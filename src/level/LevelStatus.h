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
        std::string m_codename;
        std::string m_levelName;
        std::string m_savedMoves;
    private:
        static int script_status_readMoves(lua_State *L) throw();
        ScriptState *createScript();
        std::string getSolutionFilename();
    public:
        LevelStatus() { m_complete = false; }
        void setCodename(const std::string codename) { m_codename = codename; }
        void setLevelName(const std::string levelName)
        { m_levelName = levelName; }
        std::string getLevelName() const { return m_levelName; }

        void setComplete(bool complete) { m_complete = complete; }
        bool isComplete() const { return m_complete; }

        void readMoves(const std::string &moves);
        std::string readSolvedMoves();
        void writeSolvedMoves(const std::string &moves);

        static std::string getSolutionFilename(const std::string &codename);
};

#endif
