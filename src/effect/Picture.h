#ifndef HEADER_PICTURE_H
#define HEADER_PICTURE_H

class CVideo;

#include "IDrawer.h"
#include "Path.h"
#include "V2.h"


/**
 * Static picture at fixed screen position.
 */
class Picture : public IDrawer {
    protected:
        V2 m_loc;
        SDL_Surface *m_surface;
    public:
        Picture(const Path &file, const V2 &loc);
        virtual ~Picture();
        virtual void draw();
};

#endif
