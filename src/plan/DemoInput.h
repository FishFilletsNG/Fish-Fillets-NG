#ifndef HEADER_DEMOINPUT_H
#define HEADER_DEMOINPUT_H

class Keymap;
class DemoMode;

#include "InputHandler.h"

/**
 * Handle input for demo.
 */
class DemoInput : public InputHandler {
    private:
        static const int KEY_QUIT = 1;
        Keymap *m_keymap;
        DemoMode *m_demo;
    public:
        DemoInput(DemoMode *demo);
        ~DemoInput();

        virtual void keyEvent(const KeyStroke &stroke);
        virtual void mouseEvent(const MouseStroke &buttons);
};

#endif
