#ifndef HEADER_LEVELINPUT_H
#define HEADER_LEVELINPUT_H

class Level;
class Keymap;
class KeyStroke;

#include "InputHandler.h"

/**
 * Handle input for level.
 */
class LevelInput : public InputHandler {
    private:
        static const int KEY_QUIT = 1;
        static const int KEY_SWITCH = 2;
        static const int KEY_SAVE = 3;
        static const int KEY_LOAD = 4;
        static const int KEY_RESTART = 5;
        static const int KEY_OPTIONS = 6;
        static const int KEY_SHOW_STEPS = 7;

        Level *m_level;
        Keymap *m_keymap;
    private:
        void toggleShowSteps();
    public:
        LevelInput(Level *level);
        ~LevelInput();

        virtual void keyEvent(const KeyStroke &stroke);
};

#endif
