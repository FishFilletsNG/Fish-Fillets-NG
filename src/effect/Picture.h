#ifndef HEADER_PICTURE_H
#define HEADER_PICTURE_H

class Path;

#include "Drawable.h"
#include "V2.h"

/**
 * Static picture at fixed screen position.
 */
class Picture : public Drawable {
    protected:
        V2 m_loc;
        SDL_Surface *m_surface;
    public:
        Picture(const Path &file, const V2 &loc);
        Picture(SDL_Surface *new_surface, const V2 &loc);
        virtual ~Picture();
        virtual void drawOn(SDL_Surface *screen);

        int getW() const { return m_surface->w; }
        int getH() const { return m_surface->h; }
};

#endif
