#ifndef HEADER_STATEINPUT_H
#define HEADER_STATEINPUT_H

class Keymap;
class GameState;

#include "InputHandler.h"

/**
 * Handle input for game state.
 */
class StateInput : public InputHandler {
    protected:
        static const int KEY_QUIT = 1;
        static const int KEY_CONSOLE = 2;
        static const int KEY_MENU = 3;
        Keymap *m_keymap;
        GameState *m_state;
    protected:
        virtual void quitState();
        virtual void enableConsole();
        virtual void enableMenu() {};
        virtual void specStroke(const KeyStroke &/*stroke*/) {}
        virtual void specKey(int keyIndex);
    public:
        StateInput(GameState *state);
        ~StateInput();

        virtual void keyEvent(const KeyStroke &stroke);
};

#endif
