#ifndef HEADER_DEMOINPUT_H
#define HEADER_DEMOINPUT_H

class GameState;

#include "GameInput.h"

/**
 * Handle input for demo.
 */
class DemoInput : public GameInput {
    protected:
        virtual void enableHelp() {}
    public:
        DemoInput(GameState *demo);

        virtual void mouseEvent(const MouseStroke &buttons);
};

#endif
