#ifndef HEADER_LEVELSTATUS_H
#define HEADER_LEVELSTATUS_H

/**
 * Status of level after a game.
 */
class LevelStatus {
    private:
        bool m_complete;
    public:
        LevelStatus() { m_complete = false; }
        void setComplete(bool complete) { m_complete = complete; }
        bool isComplete() const { return m_complete; }
};

#endif
