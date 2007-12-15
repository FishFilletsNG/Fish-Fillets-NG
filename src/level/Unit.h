#ifndef HEADER_UNIT_H
#define HEADER_UNIT_H

class Cube;
class InputProvider;

#include "KeyControl.h"
#include "ControlSym.h"
#include "Dir.h"
#include "V2.h"

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
        char myOrder(Dir::eDir dir) const;
        char driveOrder(char move);

        bool isMoving() const;
        bool isMovingDown() const;
        bool isTurning() const;
        bool isPushing() const;
        bool isDrivenBy(char symbol) const;
        bool equalsModel(const Cube *other) const;
        V2 getLoc() const;
        int getW() const;
        int getH() const;
        bool isFreePlace(const V2 &loc) const;

        int countAnimPhases(const std::string &anim) const;
        bool isPowerful() const;
};

#endif
