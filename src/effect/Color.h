#ifndef HEADER_COLOR_H
#define HEADER_COLOR_H

#include "SDL.h"

/**
 * Constructor for SDL_Color.
 */
class Color : public SDL_Color {
    public:
        Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha=255)
        {
            this->r = red;
            this->g = green;
            this->b = blue;
            this->unused = alpha;
        }
};

#endif
