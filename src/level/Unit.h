#ifndef HEADER_UNIT_H
#define HEADER_UNIT_H

class Cube;

#include "KeyControl.h"
#include "ControlSym.h"

#include "SDL.h"

/**
 * Unit to drive.
 */
class Unit {
    private:
        KeyControl m_buttons;
        ControlSym m_symbols;
        Cube *m_model;
        bool m_startActive;
    private:
        char goLeft();
        char goRight();
        char goUp();
        char goDown();
    public:
        Unit(const KeyControl &buttons, const ControlSym &symbols,
                bool startActive=false);
        void takeModel(Cube *model) { m_model = model; }
        bool startActive() { return m_startActive; }

        bool canDrive();
        bool willMove();
        char drive(Uint8 *pressed);
        char driveBorrowed(Uint8 *pressed, const KeyControl &buttons);
        void activate();

        char mySymbol(SDLKey key);
        char mySymbolBorrowed(SDLKey key, const KeyControl &buttons);
        char driveOrder(char move);

        bool isMoving();
        bool isTurning();
        bool isPushing();
};

#endif
