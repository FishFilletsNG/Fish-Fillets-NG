#ifndef HEADER_PIXELTOOL_H
#define HEADER_PIXELTOOL_H

#include "SDL.h"

/**
 * Pixel rutines.
 */
class PixelTool {
    public:
        static bool colorEquals(const SDL_Color &color1,
                const SDL_Color &color2);

        static SDL_Color getColor(SDL_Surface *surface, int x, int y);
        static void putColor(SDL_Surface *surface, int x, int y,
                const SDL_Color &color);

        static Uint32 getPixel(SDL_Surface *surface, int x, int y);
        static void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
};

#endif
