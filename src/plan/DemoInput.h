#ifndef HEADER_DEMOINPUT_H
#define HEADER_DEMOINPUT_H

class Keymap;
class DemoMode;

#include "NoCopy.h"

/**
 * Handle input for demo.
 */
class DemoInput : public NoCopy {
    private:
        Keymap *m_keymap;
    public:
        DemoInput();
        ~DemoInput();
        bool processInput(DemoMode *demo);
};

#endif
