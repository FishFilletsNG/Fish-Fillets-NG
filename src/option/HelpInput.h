#ifndef HEADER_HELPINPUT_H
#define HEADER_HELPINPUT_H

class GameState;

#include "StateInput.h"

/**
 * Handle input for help screen.
 */
class HelpInput : public StateInput {
    public:
        HelpInput(GameState *state);
        virtual void enableHelp() { quitState(); }
        virtual void enableMenu() { quitState(); }

        virtual void mouseEvent(const MouseStroke &buttons);
};

#endif
