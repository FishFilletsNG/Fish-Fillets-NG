#ifndef HEADER_WICONTAINER_H
#define HEADER_WICONTAINER_H

#include "IWidget.h"

/**
 * Widget with one subwidget inside.
 */
class WiContainer : public IWidget {
    private:
        IWidget *m_content;
        int m_border;
    protected:
        virtual void own_mouseButton(const MouseStroke &stroke);
    public:
        WiContainer(IWidget *new_content, int border=0);
        virtual ~WiContainer();
        virtual int getW() const;
        virtual int getH() const;
        virtual void setShift(const V2 &shift);

        virtual void drawOn(SDL_Surface *screen);
};

#endif
