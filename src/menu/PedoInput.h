#ifndef HEADER_PEDOINPUT_H
#define HEADER_PEDOINPUT_H

class Keymap;
class Pedometer;

#include "InputHandler.h"

/**
 * Handle input for pedometer.
 */
class PedoInput : public InputHandler {
    private:
        static const int KEY_QUIT = 1;

        Pedometer *m_pedometer;
        Keymap *m_keymap;
    public:
        PedoInput(Pedometer *pedometer);
        ~PedoInput();

        virtual void keyEvent(const KeyStroke &stroke);
        virtual void mouseEvent(const MouseStroke &buttons);
};

#endif
