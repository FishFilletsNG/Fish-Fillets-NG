#ifndef HEADER_WORLDINPUT_H
#define HEADER_WORLDINPUT_H

class Keymap;
class WorldMap;

#include "InputHandler.h"

/**
 * Handle input for worldmap.
 */
class WorldInput : public InputHandler {
    private:
        static const int KEY_QUIT = 1;

        WorldMap *m_world;
        Keymap *m_keymap;
    public:
        WorldInput(WorldMap *world);
        ~WorldInput();

        virtual void keyEvent(const KeyStroke &stroke);
        virtual void mouseEvent(const MouseStroke &buttons);
};

#endif
