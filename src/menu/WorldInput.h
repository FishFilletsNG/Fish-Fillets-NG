#ifndef HEADER_WORLDINPUT_H
#define HEADER_WORLDINPUT_H

class Keymap;
class WorldMap;

#include "GameInput.h"

/**
 * Handle input for worldmap.
 */
class WorldInput : public GameInput {
    private:
        WorldMap *getWorld();
    protected:
        virtual void enableSubtitles() {}
    public:
        WorldInput(WorldMap *world);

        virtual void mouseEvent(const MouseStroke &buttons);
};

#endif
