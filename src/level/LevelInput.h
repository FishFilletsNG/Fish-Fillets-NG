#ifndef HEADER_LEVELINPUT_H
#define HEADER_LEVELINPUT_H

class Keymap;
class Level;

#include "NoCopy.h"

/**
 * Handle input for level.
 */
class LevelInput : public NoCopy {
    private:
        Keymap *m_keymap;
    public:
        LevelInput();
        ~LevelInput();
        bool processInput(Level *level);
};

#endif
