#ifndef HEADER_LEVELCOUNTDOWN_H
#define HEADER_LEVELCOUNTDOWN_H

class LevelStatus;
class RoomAccess;
class Room;
class GameState;
class CountAdvisor;

/**
 * Countdowns finished or wrong state.
 */
class LevelCountDown {
    private:
        int m_countdown;
        const RoomAccess *m_access;
        LevelStatus *m_levelStatus;
    private:
        void setCountDown(const CountAdvisor *advisor);
    public:
        LevelCountDown(const RoomAccess *access);
        void fillStatus(LevelStatus *status) { m_levelStatus = status; }
        void reset();

        bool countDown(const CountAdvisor *advisor);
        bool isFinishedEnough() const;
        bool isWrongEnough() const;

        void saveSolution();
        GameState *createNextState();
};

#endif
