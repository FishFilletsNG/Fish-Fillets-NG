#ifndef HEADER_UNIT_H
#define HEADER_UNIT_H

class Cube;
class InputProvider;

#include "KeyControl.h"
#include "ControlSym.h"

#include "SDL.h"
#include <string>

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

        bool canDrive() const;
        bool willMove() const;
        char drive(const InputProvider *input);
        char driveBorrowed(const InputProvider *input,
                const KeyControl &buttons);
        void activate();

        char mySymbol(SDLKey key) const;
        char mySymbolBorrowed(SDLKey key, const KeyControl &buttons) const;
        char driveOrder(char move);

        bool isMoving() const;
        bool isTurning() const;
        bool isPushing() const;

        int countAnimPhases(const std::string &anim) const;
};

#endif
