#ifndef HEADER_CONTROLS_H
#define HEADER_CONTROLS_H

class Unit;
class PhaseLocker;
class KeyStroke;

#include "NoCopy.h"
#include "KeyControl.h"
#include "StepCounter.h"

#include "SDL.h"
#include <vector>
#include <string>

/**
 * Keyboard and mouse controls.
 */
class Controls : public StepCounter, public NoCopy {
    private:
        typedef std::vector<Unit*> t_units;
        t_units m_units;
        t_units::iterator m_active;
        int m_speedup;
        KeyControl m_arrows;
        Uint8 *m_pressed;
        bool m_switch;
        std::string m_moves;
        PhaseLocker *m_locker;
        char m_strokeSymbol;
    private:
        bool useSwitch();
        bool useStroke();
        void driveUnit();
        void setActive(t_units::iterator active);
    public:
        Controls(PhaseLocker *locker);
        ~Controls();
        void addUnit(Unit *unit);

        void driving();
        void lockPhases();

        void checkActive();
        void switchActive();
        void controlEvent(const KeyStroke &stroke);
        bool makeMove(char move);
        bool cannotMove();

        virtual int getStepCount() const { return m_moves.size(); }
        virtual std::string getMoves() const { return m_moves; }
};

#endif
