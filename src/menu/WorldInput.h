#ifndef HEADER_WORLDINPUT_H
#define HEADER_WORLDINPUT_H

class Keymap;
class WorldMap;

#include "NoCopy.h"

/**
 * Handle input for worldmap.
 */
class WorldInput : public NoCopy {
    private:
        Keymap *m_keymap;
    public:
        WorldInput();
        ~WorldInput();
        bool processInput(WorldMap *world);
};

#endif
