#ifndef HEADER_IWIDGET_H
#define HEADER_IWIDGET_H

class MouseStroke;

#include "Drawable.h"
#include "V2.h"

#include "SDL.h"

/**
 * Widget interface.
 */
class IWidget : public Drawable {
    protected:
        V2 m_shift;
    protected:
        virtual void own_mouseButton(const MouseStroke &/*stroke*/) {}
    public:
        IWidget() : m_shift(0, 0) {}
        virtual int getW() const = 0;
        virtual int getH() const = 0;
        virtual void setShift(const V2 &shift) { m_shift = shift; }

        void mouseButton(const MouseStroke &stroke);
        bool isInside(const V2 &loc);
};

#endif
