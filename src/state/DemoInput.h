#ifndef HEADER_DEMOINPUT_H
#define HEADER_DEMOINPUT_H

class Keymap;
class DemoMode;

#include "GameInput.h"

/**
 * Handle input for demo.
 */
class DemoInput : public GameInput {
    public:
        DemoInput(DemoMode *demo);

        virtual void mouseEvent(const MouseStroke &buttons);
};

#endif
