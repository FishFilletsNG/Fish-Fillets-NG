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
    private:
        bool canMove();
        char goLeft();
        char goRight();
        char goUp();
        char goDown();
    public:
        Unit(const KeyControl &buttons, const ControlSym &symbols);
        void takeModel(Cube *model) { m_model = model; }

        bool canDrive();
        char drive(Uint8 *pressed);
        char driveBorrowed(Uint8 *pressed, const KeyControl &buttons);
        void activate();

        char driveOrder(char move);

        bool isMoving();
        bool isTurning();
        bool isPushing();
};

#endif
