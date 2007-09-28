#ifndef HEADER_LEVELLOADING_H
#define HEADER_LEVELLOADING_H

class RoomAccess;

#include "NoCopy.h"

#include <string>

/**
 * Game loading.
 */
class LevelLoading : public NoCopy {
    private:
        static const int SPEED_REPLAY = 1;

        bool m_paused;
        bool m_replayMode;
        int m_loadSpeed;
        std::string m_loadedMoves;
        RoomAccess *m_access;
    public:
        LevelLoading(RoomAccess *access);
        void setLoadSpeed(int loadSpeed) { m_loadSpeed = loadSpeed; }
        void reset();
        void loadGame(const std::string &moves);
        void loadReplay(const std::string &moves);
        void togglePause() { m_paused = !m_paused; }
        bool isPaused() const { return m_paused; }

        bool isLoading() const;
        void nextLoadAction();
};

#endif
