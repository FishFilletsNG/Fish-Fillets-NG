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
        std::string m_tip;
    protected:
        virtual void own_mouseButton(const MouseStroke &/*stroke*/) {}
        virtual std::string own_getTip(const V2 &/*loc*/) { return m_tip; }
    public:
        IWidget() : m_shift(0, 0) {}
        virtual int getW() const = 0;
        virtual int getH() const = 0;
        virtual void setShift(const V2 &shift) { m_shift = shift; }
        virtual void setTip(const std::string &tip) { m_tip = tip; }

        void mouseButton(const MouseStroke &stroke);
        std::string getTip(const V2 &loc);
        bool isInside(const V2 &loc);
};

#endif
