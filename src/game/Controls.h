#ifndef HEADER_CONTROLS_H
#define HEADER_CONTROLS_H

class Unit;

#include "NoCopy.h"
#include "KeyControl.h"

#include "SDL.h"
#include <vector>
#include <string>

/**
 * Keyboard and mouse controls.
 */
class Controls : public NoCopy {
    private:
        typedef std::vector<Unit*> t_units;
        t_units m_units;
        t_units::iterator m_active;
        int m_speedup;
        KeyControl m_arrows;
        Uint8 *m_pressed;
        bool m_switch;
        std::string m_moves;
    private:
        bool finishSwitch();
        void driveUnit();
    public:
        Controls();
        ~Controls();
        void addUnit(Unit *unit);

        void driving();
        void lockPhases();

        void checkActive();
        void switchActive();
        std::string getMoves() const { return m_moves; }
        bool makeMove(char move);
};

#endif
