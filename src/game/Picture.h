#ifndef HEADER_PICTURE_H
#define HEADER_PICTURE_H

class CVideo;

#include "NoCopy.h"
#include "IDrawer.h"
#include "Path.h"


/**
 * Static picture at fixed screen position.
 */
class Picture : public IDrawer, public NoCopy {
    private:
        int m_x;
        int m_y;
        SDL_Surface *m_surface;
    public:
        Picture(const Path &file, int x, int y);
        virtual ~Picture();
        virtual void draw();
};

#endif
