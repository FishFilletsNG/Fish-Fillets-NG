#ifndef HEADER_CONTROLS_H
#define HEADER_CONTROLS_H

class V2;
class Unit;
class Cube;
class PhaseLocker;
class KeyStroke;
class InputProvider;
class MouseStroke;

#include "NoCopy.h"
#include "KeyControl.h"
#include "StepCounter.h"

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
        bool m_switch;
        std::string m_moves;
        PhaseLocker *m_locker;
        char m_strokeSymbol;
    private:
        bool useSwitch();
        bool useStroke();
        bool driveUnit(const InputProvider *input);
        void setActive(t_units::iterator active);
        int getNeededPhases(int speedup) const;
    public:
        Controls(PhaseLocker *locker);
        ~Controls();
        void addUnit(Unit *unit);

        bool driving(const InputProvider *input);
        void lockPhases();

        void checkActive();
        void switchActive();
        bool makeMove(char move);
        bool cannotMove() const;

        void controlEvent(const KeyStroke &stroke);
        void controlField(const V2 &field, const Cube *occupant);
        void controlFieldHard(const V2 &field, const Cube *occupant);
        bool activateSelected(const Cube *occupant);
        void moveTo(const V2 &loc);
        void moveHardTo(const V2 &loc);

        virtual int getStepCount() const { return m_moves.size(); }
        virtual std::string getMoves() const { return m_moves; }
};

#endif
