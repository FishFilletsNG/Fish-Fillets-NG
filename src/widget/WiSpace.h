#ifndef HEADER_WISPACE_H
#define HEADER_WISPACE_H

#include "IWidget.h"

/**
 * Empty place with width and height.
 */
class WiSpace : public IWidget {
    private:
        int m_w;
        int m_h;
    public:
        WiSpace(int width, int height) : m_w(width), m_h(height) {}
        virtual int getW() const { return m_w; }
        virtual int getH() const { return m_h; }

        virtual void drawOn(SDL_Surface * /*screen*/) {}
};

#endif
