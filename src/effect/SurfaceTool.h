#ifndef HEADER_SURFACETOOL_H
#define HEADER_SURFACETOOL_H

#include "SDL.h"

/**
 * Surface rutines.
 */
class SurfaceTool {
    public:
        static SDL_Surface *createEmpty(SDL_Surface *surface,
                int width=0, int height=0);
        static SDL_Surface *createTransparent(int w, int h,
                const SDL_Color &transparent);
        static SDL_Surface *createClone(SDL_Surface *surface);
        static void alphaFill(SDL_Surface *surface, SDL_Rect *dstrect,
                const SDL_Color &color);
};

#endif
