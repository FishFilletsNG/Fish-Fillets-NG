#ifndef HEADER_DECOR_H
#define HEADER_DECOR_H

class View;

#include "SDL.h"

/**
 * Screen decoration.
 */
class Decor {
    public:
        virtual ~Decor() {}
        virtual void drawOnScreen(const View *view, SDL_Surface *screen) = 0;
};

#endif
