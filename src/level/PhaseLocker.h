#ifndef HEADER_PHASELOCKER_H
#define HEADER_PHASELOCKER_H

/**
 * Lock game for anim phases.
 */
class PhaseLocker {
    private:
        int m_lockPhases;
    public:
        PhaseLocker();
        void reset() { m_lockPhases = 0; }
        int getLocked() const { return m_lockPhases; }
        void decLock();
        void ensurePhases(int count);
};

#endif
