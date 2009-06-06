#ifndef HEADER_VIEWEFFECT_H
#define HEADER_VIEWEFFECT_H

#include "SDL.h"

/**
 * Graphic effect before blit.
 */
class ViewEffect {
    public:
        virtual ~ViewEffect() {}
        /**
         * Update effect after blit.
         */
        virtual void updateEffect() {}
        virtual const char* getName() const = 0;
        virtual bool isDisintegrated() const { return false; }
        virtual bool isInvisible() const { return false; }
        virtual void blit(SDL_Surface *screen, SDL_Surface *surface,
                int x, int y) = 0;
};

#endif
