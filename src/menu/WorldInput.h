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
        static const int KEY_TAB = 101;
        static const int KEY_ENTER = 102;
    private:
        WorldMap *getWorld();
    protected:
        virtual void enableSubtitles() {}
        virtual void enableHelp() {}
        virtual void specKey(int keyIndex);
    public:
        WorldInput(WorldMap *world);

        virtual void mouseEvent(const MouseStroke &buttons);
};

#endif
