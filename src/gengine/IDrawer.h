#ifndef HEADER_IDRAWER_H
#define HEADER_IDRAWER_H

#include "SDL.h"

/**
 * Interface - drawable object.
 */
class IDrawer {
    protected:
        SDL_Surface *m_screen;
    public:
        IDrawer();
        virtual ~IDrawer();
        virtual void draw() = 0;
        void takeScreen(SDL_Surface *screen) { m_screen = screen; }
};

#endif
