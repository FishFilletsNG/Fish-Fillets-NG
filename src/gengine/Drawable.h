#ifndef HEADER_DRAWABLE_H
#define HEADER_DRAWABLE_H

#include "NoCopy.h"

#include "SDL.h"

/**
 * Interface - draw able object.
 */
class Drawable : public NoCopy {
    public:
        virtual ~Drawable() {}
        virtual void drawOn(SDL_Surface *screen) = 0;
};

#endif
