#ifndef HEADER_LEVELCOUNTDOWN_H
#define HEADER_LEVELCOUNTDOWN_H

class LevelStatus;
class RoomAccess;
class Room;
class GameState;

/**
 * Countdowns finished or wrong state.
 */
class LevelCountDown {
    private:
        int m_countdown;
        const RoomAccess *m_access;
        LevelStatus *m_levelStatus;
    private:
        void setCountDown(bool fast);
    public:
        LevelCountDown(const RoomAccess *access);
        void fillStatus(LevelStatus *status) { m_levelStatus = status; }
        void reset();

        bool countDown(bool fast);
        bool isFinishedEnough() const;
        bool isWrongEnough() const;

        void saveSolution();
        GameState *createNextState();
};

#endif
