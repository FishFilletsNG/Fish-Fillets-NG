#ifndef HEADER_IDRAWER_H
#define HEADER_IDRAWER_H

#include "NoCopy.h"

#include "SDL.h"

/**
 * Interface - drawable object.
 */
class IDrawer : public NoCopy {
    private:
        bool m_active;
    protected:
        SDL_Surface *m_screen;
    public:
        IDrawer();
        virtual ~IDrawer();
        virtual void draw() = 0;

        void activate();
        void deactivate();
        bool isActive() const { return m_active; }

        void takeScreen(SDL_Surface *screen) { m_screen = screen; }
};

#endif
