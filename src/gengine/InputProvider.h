#ifndef HEADER_INPUTPROVIDER_H
#define HEADER_INPUTPROVIDER_H

#include "V2.h"

#include "SDL.h"

/**
 * Interface to pressed keys and mouse location.
 */
class InputProvider {
    public:
        virtual ~InputProvider() {}
        virtual bool isPressed(SDLKey key) const = 0;
        virtual V2 getMouseLoc() const = 0;
};

#endif
