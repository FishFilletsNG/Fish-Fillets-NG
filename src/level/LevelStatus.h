#ifndef HEADER_LEVELSTATUS_H
#define HEADER_LEVELSTATUS_H

class GameState;

#include "Scripter.h"

/**
 * Status of level after a game.
 */
class LevelStatus : public Scripter {
    private:
        bool m_complete;
        bool m_wasRunning;
        std::string m_codename;
        std::string m_poster;
        std::string m_savedMoves;
        int m_bestMoves;
        std::string m_bestAuthor;
    private:
        std::string getSolutionFilename() const;
    public:
        LevelStatus();
        void prepareRun(const std::string &codename,
                const std::string &poster,
                int bestMoves, const std::string &bestAuthor);
        GameState *createPoster() const;
        int getBestMoves() const { return m_bestMoves; }
        std::string getBestAuthor() const { return m_bestAuthor; }
        int compareToBest();

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
